#include "YYSWantedTask.h"

#include <chrono>
#include <thread>

#include "Base/IActionExecutor.h"
#include "Base/ITemplateResolver.h"
#include "Base/YYSContext.h"
#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

YYSWantedTask::YYSWantedTask()
    : m_priority(60)
    , m_tasks_completed(0)
{
    YYS_LOG_INFO("YYSWantedTask constructed, max tasks: %d", m_max_tasks);
}

bool YYSWantedTask::_run()
{
    YYS_LOG_INFO("========== YYSWantedTask started ==========");

    if (!check_enable()) {
        YYS_LOG_ERROR("Enable check failed");
        return false;
    }

    if (!check_resource()) {
        YYS_LOG_ERROR("Resource check failed");
        return false;
    }

    m_navigator = std::make_unique<WantedNavigator>(m_ctx);
    m_recognizer = std::make_unique<WantedTaskRecognizer>(m_ctx);
    m_reward_handler = std::make_unique<WantedRewardHandler>(m_ctx);

    if (!m_navigator->enter()) {
        YYS_LOG_ERROR("Failed to enter wanted interface");
        return false;
    }

    for (int i = 0; i < m_max_tasks; ++i) {
        YYS_LOG_INFO("Searching for wanted task #%d", i + 1);

        if (!m_recognizer->find_and_accept_task()) {
            YYS_LOG_INFO("No more wanted tasks available");
            break;
        }

        if (!m_reward_handler->wait_and_handle()) {
            YYS_LOG_ERROR("Failed to complete wanted task #%d", i + 1);
            return false;
        }

        ++m_tasks_completed;
        YYS_LOG_INFO("Wanted task #%d completed", m_tasks_completed);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    YYS_LOG_INFO("========== YYSWantedTask completed, total: %d ==========", m_tasks_completed);
    return true;
}

bool YYSWantedTask::on_executed()
{
    YYS_LOG_INFO("YYSWantedTask on_executed, tasks completed: %d", m_tasks_completed);
    return true;
}

} // namespace asst::yys
