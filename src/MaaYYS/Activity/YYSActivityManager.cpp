#include "YYSActivityManager.h"
#include <algorithm>

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
        if (activity) {
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

} // namespace asst::yys
