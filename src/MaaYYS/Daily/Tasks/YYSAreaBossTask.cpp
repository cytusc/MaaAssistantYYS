#include "YYSAreaBossTask.h"

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

namespace asst::yys {

YYSAreaBossTask::YYSAreaBossTask()
    : m_priority(45)
{
    YYS_LOG_INFO("YYSAreaBossTask constructed");
}

bool YYSAreaBossTask::_run()
{
    YYS_LOG_INFO("========== YYSAreaBossTask started ==========");

    if (!check_resource()) {
        YYS_LOG_ERROR("Resource check failed");
        return false;
    }

    // Detect area boss entrance
    YYSWaitFlow detect("I_AREA_BOSS_ENTER", DETECT_TIMEOUT_MS);
    detect.init(m_ctx);
    if (!detect.run()) {
        YYS_LOG_INFO("Area boss entrance not detected, skipping");
        return true;
    }

    YYS_LOG_INFO("Area boss entrance detected, clicking...");
    YYSRetryFlow click_enter(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_AREA_BOSS_ENTER"),
        3);
    click_enter.init(m_ctx);
    click_enter.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Click challenge button
    YYSWaitFlow wait_battle("I_AREA_BOSS_BATTLE", 5000);
    wait_battle.init(m_ctx);
    if (wait_battle.run()) {
        YYSRetryFlow click_battle(
            std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_AREA_BOSS_BATTLE"),
            3);
        click_battle.init(m_ctx);
        click_battle.run();
    }

    // Wait for reward (2 minute timeout for boss battle)
    YYS_LOG_INFO("Waiting for area boss battle to end (2min timeout)...");
    YYSWaitFlow wait_reward("I_AREA_BOSS_REWARD", BATTLE_TIMEOUT_MS);
    wait_reward.init(m_ctx);
    if (!wait_reward.run()) {
        YYS_LOG_ERROR("Area boss reward not detected (timeout)");
        return false;
    }

    // Click reward to dismiss
    int attempts = 0;
    while (attempts < 5) {
        YYSWaitFlow check("I_AREA_BOSS_REWARD", 2000);
        check.init(m_ctx);
        if (!check.run()) break;

        YYSActionFlow click(ActionType::Click, 0, 0, "I_AREA_BOSS_REWARD");
        click.init(m_ctx);
        click.run();
        ++attempts;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    YYS_LOG_INFO("========== YYSAreaBossTask completed ==========");
    return true;
}

} // namespace asst::yys
