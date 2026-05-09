#include "YYSExplorationTask.h"

#include <chrono>
#include <thread>

#include "Base/IActionExecutor.h"
#include "Base/ITemplateResolver.h"
#include "Base/YYSContext.h"
#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

YYSExplorationTask::YYSExplorationTask()
    : m_priority(40)
{
    YYS_LOG_INFO("YYSExplorationTask constructed, target chapter: %d", m_target_chapter);
}

bool YYSExplorationTask::_run()
{
    YYS_LOG_INFO("========== YYSExplorationTask started ==========");
    YYS_LOG_INFO("Target chapter: %d, Max nodes: %d", m_target_chapter, m_max_nodes);

    if (!check_enable()) {
        YYS_LOG_ERROR("Enable check failed");
        return false;
    }

    if (!check_resource()) {
        YYS_LOG_ERROR("Resource check failed");
        return false;
    }

    m_navigator = std::make_unique<ExplorationNavigator>(m_ctx);
    m_chapter_selector = std::make_unique<ChapterSelector>(m_ctx);
    m_runner = std::make_unique<ExplorationRunner>(m_ctx);
    m_reward_handler = std::make_unique<ExplorationRewardHandler>(m_ctx);

    m_runner->set_max_nodes(m_max_nodes);

    if (!m_navigator->enter()) {
        YYS_LOG_ERROR("Failed to enter exploration interface");
        return false;
    }

    if (!m_chapter_selector->select(m_target_chapter)) {
        YYS_LOG_ERROR("Failed to select chapter: %d", m_target_chapter);
        return false;
    }

    if (!m_runner->enter_chapter()) {
        YYS_LOG_ERROR("Failed to enter chapter");
        return false;
    }

    if (!m_runner->explore_loop()) {
        YYS_LOG_ERROR("Exploration loop failed");
        return false;
    }

    m_reward_handler->handle();

    YYS_LOG_INFO("========== YYSExplorationTask completed ==========");
    return true;
}

bool YYSExplorationTask::on_executed()
{
    YYS_LOG_INFO("YYSExplorationTask on_executed called");
    return true;
}

} // namespace asst::yys
