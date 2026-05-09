#include "ConfigDrivenActivity.h"

#include <fstream>
#include <thread>
#include <chrono>

#include "Base/IActionExecutor.h"
#include "Base/ITemplateResolver.h"
#include "Base/YYSContext.h"
#include "Base/YYSTypes.h"
#include "Common/Logger/YYSLogger.h"
#include "Common/Flow/YYSWaitFlow.h"
#include "Common/Flow/YYSActionFlow.h"
#include "Common/Flow/YYSRetryFlow.h"

#include "meojson/json.hpp"

namespace asst::yys {

ConfigDrivenActivity::ConfigDrivenActivity(std::string id, std::string name,
                                           std::vector<ActivityStep> steps)
    : m_id(std::move(id))
    , m_name(std::move(name))
    , m_steps(std::move(steps))
{
}

bool ConfigDrivenActivity::_run()
{
    YYS_LOG_INFO("========== Activity [%s] started ==========", m_name.c_str());

    for (size_t i = 0; i < m_steps.size(); ++i) {
        const auto& step = m_steps[i];
        YYS_LOG_INFO("Step %zu/%zu: %s", i + 1, m_steps.size(), step.name.c_str());

        if (!execute_step(step)) {
            YYS_LOG_ERROR("Step failed: %s", step.name.c_str());
            return false;
        }
    }

    YYS_LOG_INFO("========== Activity [%s] completed ==========", m_name.c_str());
    return true;
}

bool ConfigDrivenActivity::execute_step(const ActivityStep& step)
{
    if (!m_ctx) return false;

    if (step.action == "wait") {
        YYSWaitFlow wait(step.target, step.timeout);
        wait.init(m_ctx);
        return wait.run();
    }

    if (step.action == "click") {
        YYSRetryFlow click(
            std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, step.target),
            3);
        click.init(m_ctx);
        bool ok = click.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        return ok;
    }

    if (step.action == "wait_click") {
        YYSWaitFlow wait(step.target, step.timeout);
        wait.init(m_ctx);
        if (!wait.run()) return false;

        YYSRetryFlow click(
            std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, step.target),
            3);
        click.init(m_ctx);
        bool ok = click.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        return ok;
    }

    YYS_LOG_WARN("Unknown action: %s", step.action.c_str());
    return false;
}

std::unique_ptr<ConfigDrivenActivity> load_activity_from_config(
    const std::filesystem::path& config_path)
{
    std::ifstream ifs(config_path);
    if (!ifs.is_open()) return nullptr;

    std::string content((std::istreambuf_iterator<char>(ifs)),
                         std::istreambuf_iterator<char>());
    auto opt = json::parse(content);
    if (!opt.has_value()) return nullptr;
    const auto& root = opt.value();

    std::string id = root.at("activity_id").as_string();
    std::string name = root.at("activity_name").as_string();

    std::vector<ActivityStep> steps;
    if (root.contains("steps")) {
        for (const auto& step_json : root.at("steps").as_array()) {
            ActivityStep step;
            step.name = step_json.at("name").as_string();
            step.action = step_json.at("action").as_string();
            step.target = step_json.at("target").as_string();
            if (step_json.contains("timeout")) {
                step.timeout = step_json.at("timeout").as_integer();
            }
            steps.push_back(std::move(step));
        }
    }

    return std::make_unique<ConfigDrivenActivity>(
        std::move(id), std::move(name), std::move(steps));
}

} // namespace asst::yys
