#include "YYSAwakeningTask.h"

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

YYSAwakeningTask::YYSAwakeningTask()
    : m_priority(40)
{
    YYS_LOG_INFO("YYSAwakeningTask constructed");
}

bool YYSAwakeningTask::_run()
{
    YYS_LOG_INFO("========== YYSAwakeningTask started ==========");

    if (!check_resource()) {
        YYS_LOG_ERROR("Resource check failed");
        return false;
    }

    // Detect awakening entrance
    YYSWaitFlow detect("I_AWAKENING_ENTER", DETECT_TIMEOUT_MS);
    detect.init(m_ctx);
    if (!detect.run()) {
        YYS_LOG_INFO("Awakening entrance not detected, skipping");
        return true;
    }

    YYS_LOG_INFO("Awakening entrance detected, clicking...");
    YYSRetryFlow click_enter(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_AWAKENING_ENTER"),
        3);
    click_enter.init(m_ctx);
    click_enter.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Click challenge button
    YYSWaitFlow wait_battle("I_AWAKENING_BATTLE", 5000);
    wait_battle.init(m_ctx);
    if (wait_battle.run()) {
        YYSRetryFlow click_battle(
            std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_AWAKENING_BATTLE"),
            3);
        click_battle.init(m_ctx);
        click_battle.run();
    }

    // Wait for reward (2 minute timeout for battle)
    YYS_LOG_INFO("Waiting for awakening battle to end (2min timeout)...");
    YYSWaitFlow wait_reward("I_AWAKENING_REWARD", BATTLE_TIMEOUT_MS);
    wait_reward.init(m_ctx);
    if (!wait_reward.run()) {
        YYS_LOG_ERROR("Awakening reward not detected (timeout)");
        return false;
    }

    // Click reward to dismiss
    int attempts = 0;
    while (attempts < 5) {
        YYSWaitFlow check("I_AWAKENING_REWARD", 2000);
        check.init(m_ctx);
        if (!check.run()) break;

        YYSActionFlow click(ActionType::Click, 0, 0, "I_AWAKENING_REWARD");
        click.init(m_ctx);
        click.run();
        ++attempts;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    YYS_LOG_INFO("========== YYSAwakeningTask completed ==========");
    return true;
}

} // namespace asst::yys
