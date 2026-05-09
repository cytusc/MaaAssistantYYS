#include "WantedRewardHandler.h"

#include <thread>
#include <chrono>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSTypes.h"
#include "Common/Logger/YYSLogger.h"
#include "Common/Flow/YYSWaitFlow.h"
#include "Common/Flow/YYSActionFlow.h"

namespace asst::yys {

WantedRewardHandler::WantedRewardHandler(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool WantedRewardHandler::wait_and_handle()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Waiting for wanted battle to complete...");

    YYSWaitFlow wait_complete("I_WANTED_COMPLETE", BATTLE_TIMEOUT_MS);
    wait_complete.init(m_ctx);
    if (!wait_complete.run()) {
        YYS_LOG_ERROR("Wanted battle completion not detected (timeout)");
        return false;
    }

    YYS_LOG_INFO("Wanted task completed, handling reward...");

    int attempts = 0;
    constexpr int max_attempts = 10;

    while (attempts < max_attempts) {
        YYSWaitFlow wait_reward("I_WANTED_REWARD", REWARD_TIMEOUT_MS);
        wait_reward.init(m_ctx);
        if (!wait_reward.run()) {
            break;
        }

        YYSActionFlow click_reward(ActionType::Click, 0, 0, "I_WANTED_REWARD");
        click_reward.init(m_ctx);
        click_reward.run();

        ++attempts;
        YYS_LOG_INFO("Clicked wanted reward, attempt: %d", attempts);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    YYS_LOG_INFO("Wanted reward handling completed, attempts: %d", attempts);
    return true;
}

} // namespace asst::yys
