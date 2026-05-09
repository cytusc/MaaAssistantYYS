#include "ExplorationRunner.h"

#include <thread>
#include <chrono>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSTypes.h"
#include "Common/Logger/YYSLogger.h"
#include "Common/Flow/YYSWaitFlow.h"
#include "Common/Flow/YYSActionFlow.h"
#include "Common/Flow/YYSRetryFlow.h"

namespace asst::yys {

ExplorationRunner::ExplorationRunner(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool ExplorationRunner::enter_chapter()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Entering exploration chapter...");

    YYSRetryFlow click_flow(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_EXPLORE_FIGHT"),
        3);
    click_flow.init(m_ctx);
    if (!click_flow.run()) {
        YYS_LOG_ERROR("Failed to click fight button");
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    YYS_LOG_INFO("Entered chapter successfully");
    return true;
}

bool ExplorationRunner::explore_loop()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Starting exploration loop, max nodes: %d", m_max_nodes);
    m_nodes_cleared = 0;

    for (int i = 0; i < m_max_nodes; ++i) {
        m_ctx->executor()->screencap();

        // Check if exploration is complete
        YYSWaitFlow complete_check("I_EXPLORE_COMPLETE", 1000);
        complete_check.init(m_ctx);
        if (complete_check.run()) {
            YYS_LOG_INFO("Exploration complete detected after %d nodes", m_nodes_cleared);
            return true;
        }

        if (!try_click_node()) {
            YYS_LOG_WARN("No clickable node found at iteration %d", i);
            break;
        }

        if (!wait_battle_end()) {
            YYS_LOG_ERROR("Battle timeout at node %d", m_nodes_cleared + 1);
            return false;
        }

        ++m_nodes_cleared;
        YYS_LOG_INFO("Node %d cleared", m_nodes_cleared);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    YYS_LOG_INFO("Exploration loop finished, nodes cleared: %d", m_nodes_cleared);
    return m_nodes_cleared > 0;
}

bool ExplorationRunner::try_click_node()
{
    // Priority: BOSS node > normal monster node
    YYSWaitFlow boss_check("I_EXPLORE_BOSS", NODE_DETECT_TIMEOUT_MS);
    boss_check.init(m_ctx);
    if (boss_check.run()) {
        YYS_LOG_INFO("BOSS node detected, clicking...");
        YYSActionFlow click_boss(ActionType::Click, 0, 0, "I_EXPLORE_BOSS");
        click_boss.init(m_ctx);
        click_boss.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return true;
    }

    YYSWaitFlow monster_check("I_EXPLORE_MONSTER", NODE_DETECT_TIMEOUT_MS);
    monster_check.init(m_ctx);
    if (monster_check.run()) {
        YYS_LOG_INFO("Monster node detected, clicking...");
        YYSActionFlow click_monster(ActionType::Click, 0, 0, "I_EXPLORE_MONSTER");
        click_monster.init(m_ctx);
        click_monster.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return true;
    }

    return false;
}

bool ExplorationRunner::wait_battle_end()
{
    YYS_LOG_INFO("Waiting for battle to end...");

    YYSWaitFlow wait_flow("I_EXPLORE_REWARD", BATTLE_TIMEOUT_MS);
    wait_flow.init(m_ctx);
    if (!wait_flow.run()) {
        YYS_LOG_ERROR("Battle reward screen not found (timeout)");
        return false;
    }

    YYS_LOG_INFO("Battle ended, reward detected");
    return true;
}

} // namespace asst::yys
