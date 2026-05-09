#include "YYSResourceLoader.h"

#include <fstream>
#include <iterator>

namespace asst::yys {

bool YYSResourceLoader::load(const std::filesystem::path& root)
{
    m_root = root;
    m_templates.clear();
    m_ocrs.clear();

    bool ok = true;
    if (!std::filesystem::exists(root)) {
        return false;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
        if (!entry.is_regular_file() || entry.path().filename() != "config.json") {
            continue;
        }
        ok = load_file(entry.path()) && ok;
    }
    return ok;
}

bool YYSResourceLoader::load_file(const std::filesystem::path& file)
{
    std::ifstream ifs(file);
    if (!ifs.is_open()) {
        return false;
    }

    const std::string content { std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
    const auto parsed = json::parse(content);
    if (!parsed) {
        return false;
    }

    const auto& root = *parsed;
    const auto config_dir = file.parent_path();

    if (root.contains("template")) {
        for (const auto& item : root.at("template").as_array()) {
            parse_template(item, config_dir);
        }
    }

    if (root.contains("ocr")) {
        for (const auto& item : root.at("ocr").as_array()) {
            parse_ocr(item);
        }
    }

    return true;
}

void YYSResourceLoader::parse_template(const json::value& value, const std::filesystem::path& config_dir)
{
    TemplateResource resource;
    resource.id = value.at("id").as_string();
    resource.file = resolve_template_file(config_dir, value.at("file").as_string());
    const auto roi = value.at("roi").as_array();
    resource.roi = Rect {
        static_cast<int>(roi[0].as_integer()),
        static_cast<int>(roi[1].as_integer()),
        static_cast<int>(roi[2].as_integer()),
        static_cast<int>(roi[3].as_integer())
    };
    if (value.contains("threshold")) {
        resource.threshold = value.at("threshold").as_double();
    }
    if (value.contains("method")) {
        resource.method = value.at("method").as_string();
    }
    if (value.contains("description")) {
        resource.description = value.at("description").as_string();
    }
    m_templates.insert_or_assign(resource.id, std::move(resource));
}

void YYSResourceLoader::parse_ocr(const json::value& value)
{
    OcrResource resource;
    resource.id = value.at("id").as_string();
    const auto roi = value.at("roi").as_array();
    resource.roi = Rect {
        static_cast<int>(roi[0].as_integer()),
        static_cast<int>(roi[1].as_integer()),
        static_cast<int>(roi[2].as_integer()),
        static_cast<int>(roi[3].as_integer())
    };
    if (value.contains("mode")) {
        resource.mode = value.at("mode").as_string();
    }
    if (value.contains("keywords")) {
        for (const auto& item : value.at("keywords").as_array()) {
            resource.keywords.push_back(item.as_string());
        }
    }
    if (value.contains("description")) {
        resource.description = value.at("description").as_string();
    }
    m_ocrs.insert_or_assign(resource.id, std::move(resource));
}

std::filesystem::path YYSResourceLoader::resolve_template_file(const std::filesystem::path& config_dir, const std::string& file) const
{
    const std::filesystem::path raw(file);
    if (raw.is_absolute()) {
        return raw;
    }

    const auto from_config = config_dir / raw;
    if (std::filesystem::exists(from_config)) {
        return from_config;
    }

    const auto from_template_dir = config_dir / "template" / raw;
    if (std::filesystem::exists(from_template_dir)) {
        return from_template_dir;
    }

    return m_root / raw;
}

} // namespace asst::yys
