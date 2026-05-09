#include "YYSTrueSnakeTask.h"

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

YYSTrueSnakeTask::YYSTrueSnakeTask()
    : m_priority(45)
{
    YYS_LOG_INFO("YYSTrueSnakeTask constructed");
}

bool YYSTrueSnakeTask::_run()
{
    YYS_LOG_INFO("========== YYSTrueSnakeTask started ==========");

    if (!check_resource()) {
        YYS_LOG_ERROR("Resource check failed");
        return false;
    }

    // Detect true snake entrance
    YYSWaitFlow detect("I_TRUE_SNAKE_ENTER", DETECT_TIMEOUT_MS);
    detect.init(m_ctx);
    if (!detect.run()) {
        YYS_LOG_INFO("True snake entrance not detected, skipping");
        return true;
    }

    YYS_LOG_INFO("True snake entrance detected, clicking...");
    YYSRetryFlow click_enter(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_TRUE_SNAKE_ENTER"),
        3);
    click_enter.init(m_ctx);
    click_enter.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Click challenge button
    YYSWaitFlow wait_battle("I_TRUE_SNAKE_BATTLE", 5000);
    wait_battle.init(m_ctx);
    if (wait_battle.run()) {
        YYSRetryFlow click_battle(
            std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_TRUE_SNAKE_BATTLE"),
            3);
        click_battle.init(m_ctx);
        click_battle.run();
    }

    // Wait for reward (2 minute timeout for boss battle)
    YYS_LOG_INFO("Waiting for true snake battle to end (2min timeout)...");
    YYSWaitFlow wait_reward("I_TRUE_SNAKE_REWARD", BATTLE_TIMEOUT_MS);
    wait_reward.init(m_ctx);
    if (!wait_reward.run()) {
        YYS_LOG_ERROR("True snake reward not detected (timeout)");
        return false;
    }

    // Click reward to dismiss
    int attempts = 0;
    while (attempts < 5) {
        YYSWaitFlow check("I_TRUE_SNAKE_REWARD", 2000);
        check.init(m_ctx);
        if (!check.run()) break;

        YYSActionFlow click(ActionType::Click, 0, 0, "I_TRUE_SNAKE_REWARD");
        click.init(m_ctx);
        click.run();
        ++attempts;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    YYS_LOG_INFO("========== YYSTrueSnakeTask completed ==========");
    return true;
}

} // namespace asst::yys
