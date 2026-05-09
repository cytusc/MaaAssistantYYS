#include "YYSNianBeastTask.h"

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

YYSNianBeastTask::YYSNianBeastTask()
    : m_priority(35)
{
    YYS_LOG_INFO("YYSNianBeastTask constructed");
}

bool YYSNianBeastTask::_run()
{
    YYS_LOG_INFO("========== YYSNianBeastTask started ==========");

    if (!check_resource()) {
        YYS_LOG_ERROR("Resource check failed");
        return false;
    }

    // Detect nian beast entrance
    YYSWaitFlow detect("I_NIAN_BEAST_ENTER", DETECT_TIMEOUT_MS);
    detect.init(m_ctx);
    if (!detect.run()) {
        YYS_LOG_INFO("Nian beast entrance not detected, skipping");
        return true;
    }

    YYS_LOG_INFO("Nian beast entrance detected, clicking...");
    YYSRetryFlow click_enter(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_NIAN_BEAST_ENTER"),
        3);
    click_enter.init(m_ctx);
    click_enter.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Click battle button
    YYSWaitFlow wait_battle("I_NIAN_BEAST_BATTLE", 5000);
    wait_battle.init(m_ctx);
    if (wait_battle.run()) {
        YYSRetryFlow click_battle(
            std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_NIAN_BEAST_BATTLE"),
            3);
        click_battle.init(m_ctx);
        click_battle.run();
    }

    // Wait for reward
    YYS_LOG_INFO("Waiting for nian beast battle to end...");
    YYSWaitFlow wait_reward("I_NIAN_BEAST_REWARD", BATTLE_TIMEOUT_MS);
    wait_reward.init(m_ctx);
    if (!wait_reward.run()) {
        YYS_LOG_ERROR("Nian beast reward not detected (timeout)");
        return false;
    }

    // Click reward to dismiss
    int attempts = 0;
    while (attempts < 5) {
        YYSWaitFlow check("I_NIAN_BEAST_REWARD", 2000);
        check.init(m_ctx);
        if (!check.run()) break;

        YYSActionFlow click(ActionType::Click, 0, 0, "I_NIAN_BEAST_REWARD");
        click.init(m_ctx);
        click.run();
        ++attempts;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    YYS_LOG_INFO("========== YYSNianBeastTask completed ==========");
    return true;
}

} // namespace asst::yys
