#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <filesystem>

namespace asst::yys {

class YYSConfig {
public:
    using Value = std::variant<int, double, bool, std::string>;

    YYSConfig() = default;

    static std::shared_ptr<YYSConfig> from_file(const std::filesystem::path& path);
    static std::shared_ptr<YYSConfig> from_defaults();

    int get_int(const std::string& key, int default_val = 0) const;
    double get_double(const std::string& key, double default_val = 0.0) const;
    bool get_bool(const std::string& key, bool default_val = false) const;
    std::string get_string(const std::string& key, const std::string& default_val = "") const;

    void set_int(const std::string& key, int value);
    void set_double(const std::string& key, double value);
    void set_bool(const std::string& key, bool value);
    void set_string(const std::string& key, const std::string& value);

    bool has(const std::string& key) const;
    void remove(const std::string& key);

    bool is_task_enabled(const std::string& task_id) const;
    void set_task_enabled(const std::string& task_id, bool enabled);

    int get_task_priority(const std::string& task_id) const;
    void set_task_priority(const std::string& task_id, int priority);

    int target_layer() const { return get_int("orochi.target_layer", 8); }
    void set_target_layer(int layer) { set_int("orochi.target_layer", layer); }

    bool sweep_enabled() const { return get_bool("orochi.sweep_enabled", false); }
    void set_sweep_enabled(bool enabled) { set_bool("orochi.sweep_enabled", enabled); }

    int sweep_count() const { return get_int("orochi.sweep_count", 0); }
    void set_sweep_count(int count) { set_int("orochi.sweep_count", count); }

    int screenshot_interval() const { return get_int("global.screenshot_interval", 200); }
    int default_timeout() const { return get_int("global.default_timeout", 30000); }
    int max_retry() const { return get_int("global.max_retry", 3); }

private:
    std::unordered_map<std::string, Value> m_values;
    std::unordered_set<std::string> m_disabled_tasks;
    std::unordered_map<std::string, int> m_task_priorities;
};

} // namespace asst::yys
