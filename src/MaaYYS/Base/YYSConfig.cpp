#include "YYSConfig.h"

#include <fstream>
#include <iterator>

#include <meojson/json.hpp>
#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

std::shared_ptr<YYSConfig> YYSConfig::from_file(const std::filesystem::path& path)
{
    auto config = std::make_shared<YYSConfig>();

    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        YYS_LOG_WARN("YYSConfig: failed to open config file: %s", path.string().c_str());
        return config;
    }

    try {
        const std::string content { std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
        const auto parsed = json::parse(content);
        if (!parsed) {
            YYS_LOG_WARN("YYSConfig: failed to parse config file");
            return config;
        }

        const auto& root = *parsed;
        if (!root.is_object()) {
            YYS_LOG_WARN("YYSConfig: root is not an object");
            return config;
        }

        for (const auto& [key, val] : root.as_object()) {
            if (val.is_number()) {
                config->set_int(key, static_cast<int>(val.as_integer()));
            }
            else if (val.is_boolean()) {
                config->set_bool(key, val.as_boolean());
            }
            else if (val.is_string()) {
                config->set_string(key, val.as_string());
            }
        }

        if (root.contains("tasks") && root.at("tasks").is_object()) {
            for (const auto& [task_id, task_val] : root.at("tasks").as_object()) {
                if (task_val.is_object()) {
                    if (task_val.contains("enabled") && task_val.at("enabled").is_boolean()) {
                        config->set_task_enabled(task_id, task_val.at("enabled").as_boolean());
                    }
                    if (task_val.contains("priority") && task_val.at("priority").is_number()) {
                        config->set_task_priority(task_id, static_cast<int>(task_val.at("priority").as_integer()));
                    }
                }
            }
        }
    }
    catch (const std::exception& e) {
        YYS_LOG_ERROR("YYSConfig: parse error: %s", e.what());
    }

    return config;
}

std::shared_ptr<YYSConfig> YYSConfig::from_defaults()
{
    auto config = std::make_shared<YYSConfig>();
    config->set_int("orochi.target_layer", 8);
    config->set_bool("orochi.sweep_enabled", false);
    config->set_int("orochi.sweep_count", 0);
    config->set_int("global.screenshot_interval", 200);
    config->set_int("global.default_timeout", 30000);
    config->set_int("global.max_retry", 3);
    return config;
}

int YYSConfig::get_int(const std::string& key, int default_val) const
{
    auto it = m_values.find(key);
    if (it == m_values.end()) return default_val;
    if (auto* p = std::get_if<int>(&it->second)) return *p;
    if (auto* p = std::get_if<double>(&it->second)) return static_cast<int>(*p);
    return default_val;
}

double YYSConfig::get_double(const std::string& key, double default_val) const
{
    auto it = m_values.find(key);
    if (it == m_values.end()) return default_val;
    if (auto* p = std::get_if<double>(&it->second)) return *p;
    if (auto* p = std::get_if<int>(&it->second)) return static_cast<double>(*p);
    return default_val;
}

bool YYSConfig::get_bool(const std::string& key, bool default_val) const
{
    auto it = m_values.find(key);
    if (it == m_values.end()) return default_val;
    if (auto* p = std::get_if<bool>(&it->second)) return *p;
    return default_val;
}

std::string YYSConfig::get_string(const std::string& key, const std::string& default_val) const
{
    auto it = m_values.find(key);
    if (it == m_values.end()) return default_val;
    if (auto* p = std::get_if<std::string>(&it->second)) return *p;
    return default_val;
}

void YYSConfig::set_int(const std::string& key, int value)
{
    m_values[key] = value;
}

void YYSConfig::set_double(const std::string& key, double value)
{
    m_values[key] = value;
}

void YYSConfig::set_bool(const std::string& key, bool value)
{
    m_values[key] = value;
}

void YYSConfig::set_string(const std::string& key, const std::string& value)
{
    m_values[key] = value;
}

bool YYSConfig::has(const std::string& key) const
{
    return m_values.find(key) != m_values.end();
}

void YYSConfig::remove(const std::string& key)
{
    m_values.erase(key);
}

bool YYSConfig::is_task_enabled(const std::string& task_id) const
{
    return m_disabled_tasks.find(task_id) == m_disabled_tasks.end();
}

void YYSConfig::set_task_enabled(const std::string& task_id, bool enabled)
{
    if (enabled) {
        m_disabled_tasks.erase(task_id);
    }
    else {
        m_disabled_tasks.insert(task_id);
    }
}

int YYSConfig::get_task_priority(const std::string& task_id) const
{
    auto it = m_task_priorities.find(task_id);
    return it != m_task_priorities.end() ? it->second : 0;
}

void YYSConfig::set_task_priority(const std::string& task_id, int priority)
{
    m_task_priorities[task_id] = priority;
}

} // namespace asst::yys
