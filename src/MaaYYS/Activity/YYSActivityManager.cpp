#include "YYSActivityManager.h"
#include "ConfigDrivenActivity.h"
#include "Base/YYSContext.h"
#include "Common/Logger/YYSLogger.h"
#include <algorithm>
#include <filesystem>

namespace asst::yys {

void YYSActivityManager::register_activity(std::unique_ptr<IYYSActivity> activity)
{
    if (activity) {
        m_activities.push_back(std::move(activity));
    }
}

bool YYSActivityManager::run_all()
{
    for (auto& activity : m_activities) {
        if (activity && is_enabled(activity->activity_id())) {
            activity->run();
        }
    }
    return true;
}

std::vector<std::string> YYSActivityManager::get_tasks() const
{
    std::vector<std::string> tasks;
    for (const auto& activity : m_activities) {
        if (activity) {
            tasks.push_back(activity->activity_id());
        }
    }
    return tasks;
}

void YYSActivityManager::set_enabled(const std::string& activity_id, bool enabled)
{
    if (enabled) {
        m_disabled.erase(activity_id);
    } else {
        m_disabled.insert(activity_id);
    }
}

bool YYSActivityManager::is_enabled(const std::string& activity_id) const
{
    return m_disabled.find(activity_id) == m_disabled.end();
}

bool YYSActivityManager::discover_and_register(
    const std::filesystem::path& activity_dir,
    std::shared_ptr<YYSContext> ctx)
{
    if (!std::filesystem::exists(activity_dir)) {
        YYS_LOG_WARN("Activity directory does not exist: %s", activity_dir.string().c_str());
        return false;
    }

    int count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(activity_dir)) {
        if (!entry.is_directory()) continue;

        auto config_path = entry.path() / "config.json";
        if (!std::filesystem::exists(config_path)) continue;

        auto activity = load_activity_from_config(config_path);
        if (!activity) continue;

        activity->init(ctx);
        YYS_LOG_INFO("Discovered activity: %s (%s)",
                     activity->activity_id().c_str(),
                     activity->activity_name().c_str());
        register_activity(std::move(activity));
        ++count;
    }

    YYS_LOG_INFO("Discovered %d activities from %s", count, activity_dir.string().c_str());
    return count > 0;
}

} // namespace asst::yys
