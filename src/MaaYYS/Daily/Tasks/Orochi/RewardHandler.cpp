#include "RewardHandler.h"

#include <thread>
#include <chrono>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSTypes.h"
#include "Common/Logger/YYSLogger.h"
#include "Common/Flow/YYSWaitFlow.h"
#include "Common/Flow/YYSActionFlow.h"
#include "Common/Flow/YYSLoopFlow.h"

namespace asst::yys {

RewardHandler::RewardHandler(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool RewardHandler::handle()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Handling reward...");

    int attempts = 0;
    constexpr int max_attempts = 10;

    while (attempts < max_attempts) {
        YYSWaitFlow wait_flow("I_REWARD", DEFAULT_TIMEOUT_MS);
        wait_flow.init(m_ctx);
        if (!wait_flow.run()) {
            break;
        }

        YYSActionFlow click_flow(ActionType::Click, 0, 0, "I_REWARD");
        click_flow.init(m_ctx);
        click_flow.run();

        ++attempts;
        YYS_LOG_INFO("Clicked reward button, attempt: %d", attempts);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    YYS_LOG_INFO("Reward handling completed, attempts: %d", attempts);
    return true;
}

} // namespace asst::yys
