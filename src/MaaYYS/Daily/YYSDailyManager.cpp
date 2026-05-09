#include "YYSDailyManager.h"
#include "Tasks/YYSOrochiTask.h"
#include <algorithm>

namespace asst::yys {

YYSDailyManager::YYSDailyManager()
{
    register_default_tasks();
}

YYSDailyManager::~YYSDailyManager()
{
}

void YYSDailyManager::register_daily(std::unique_ptr<IYYSDaily> daily)
{
    if (daily) {
        m_dailies.push_back(std::move(daily));
    }
}

bool YYSDailyManager::run_all()
{
    for (auto& daily : m_dailies) {
        if (daily && daily->check_enable()) {
            daily->run();
        }
    }
    return true;
}

bool YYSDailyManager::run_by_priority(int min_priority)
{
    for (auto& daily : m_dailies) {
        if (daily && daily->check_enable() && daily->priority() >= min_priority) {
            daily->run();
        }
    }
    return true;
}

void YYSDailyManager::register_default_tasks()
{
    // 注册默认的御魂任务
    register_daily(std::make_unique<YYSOrochiTask>());
}

} // namespace asst::yys
