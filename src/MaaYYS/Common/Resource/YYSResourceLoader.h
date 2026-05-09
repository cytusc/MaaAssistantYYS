#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include <meojson/json.hpp>

#include "Base/YYSTypes.h"

namespace asst::yys {

struct TemplateResource {
    std::string id;
    std::filesystem::path file;
    Rect roi;
    double threshold = 0.8;
    std::string method = "TemplateMatch";
    std::string description;
};

struct OcrResource {
    std::string id;
    Rect roi;
    std::string mode;
    std::vector<std::string> keywords;
    std::string description;
};

class YYSResourceLoader {
public:
    bool load(const std::filesystem::path& root);

    const std::unordered_map<std::string, TemplateResource>& templates() const noexcept { return m_templates; }
    const std::unordered_map<std::string, OcrResource>& ocrs() const noexcept { return m_ocrs; }

private:
    bool load_file(const std::filesystem::path& file);
    void parse_template(const json::value& value, const std::filesystem::path& config_dir);
    void parse_ocr(const json::value& value);
    std::filesystem::path resolve_template_file(const std::filesystem::path& config_dir, const std::string& file) const;

    std::filesystem::path m_root;
    std::unordered_map<std::string, TemplateResource> m_templates;
    std::unordered_map<std::string, OcrResource> m_ocrs;
};

} // namespace asst::yys
