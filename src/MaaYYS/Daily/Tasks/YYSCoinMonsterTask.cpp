#include "YYSCoinMonsterTask.h"

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

YYSCoinMonsterTask::YYSCoinMonsterTask()
    : m_priority(70)
{
    YYS_LOG_INFO("YYSCoinMonsterTask constructed");
}

bool YYSCoinMonsterTask::_run()
{
    YYS_LOG_INFO("========== YYSCoinMonsterTask started ==========");

    if (!check_resource()) {
        YYS_LOG_ERROR("Resource check failed");
        return false;
    }

    // Detect coin monster on screen
    YYSWaitFlow detect("I_COIN_MONSTER", DETECT_TIMEOUT_MS);
    detect.init(m_ctx);
    if (!detect.run()) {
        YYS_LOG_INFO("No coin monster detected, skipping");
        return true;
    }

    YYS_LOG_INFO("Coin monster detected, clicking...");
    YYSRetryFlow click_monster(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_COIN_MONSTER"),
        3);
    click_monster.init(m_ctx);
    click_monster.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Click battle button
    YYSWaitFlow wait_battle("I_COIN_BATTLE", 5000);
    wait_battle.init(m_ctx);
    if (wait_battle.run()) {
        YYSRetryFlow click_battle(
            std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_COIN_BATTLE"),
            3);
        click_battle.init(m_ctx);
        click_battle.run();
    }

    // Wait for reward
    YYS_LOG_INFO("Waiting for coin monster battle to end...");
    YYSWaitFlow wait_reward("I_COIN_REWARD", BATTLE_TIMEOUT_MS);
    wait_reward.init(m_ctx);
    if (!wait_reward.run()) {
        YYS_LOG_ERROR("Coin monster reward not detected (timeout)");
        return false;
    }

    // Click reward to dismiss
    int attempts = 0;
    while (attempts < 5) {
        YYSWaitFlow check("I_COIN_REWARD", 2000);
        check.init(m_ctx);
        if (!check.run()) break;

        YYSActionFlow click(ActionType::Click, 0, 0, "I_COIN_REWARD");
        click.init(m_ctx);
        click.run();
        ++attempts;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    YYS_LOG_INFO("========== YYSCoinMonsterTask completed ==========");
    return true;
}

} // namespace asst::yys
