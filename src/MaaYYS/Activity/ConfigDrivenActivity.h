#pragma once
#include "YYSBaseActivity.h"
#include <string>
#include <vector>
#include <filesystem>

namespace asst::yys {

struct ActivityStep {
    std::string name;
    std::string action;   // "wait", "click", "ocr"
    std::string target;
    int timeout = 5000;
};

class ConfigDrivenActivity : public YYSBaseActivity {
public:
    ConfigDrivenActivity(std::string id, std::string name,
                         std::vector<ActivityStep> steps);

    std::string activity_id() const override { return m_id; }
    std::string activity_name() const override { return m_name; }
    std::string name() const override { return "ConfigDrivenActivity:" + m_id; }

protected:
    bool _run() override;

private:
    bool execute_step(const ActivityStep& step);

    std::string m_id;
    std::string m_name;
    std::vector<ActivityStep> m_steps;
};

// Factory: create from config.json
std::unique_ptr<ConfigDrivenActivity> load_activity_from_config(
    const std::filesystem::path& config_path);

} // namespace asst::yys
