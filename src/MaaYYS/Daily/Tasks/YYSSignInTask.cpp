#include "YYSSignInTask.h"

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

YYSSignInTask::YYSSignInTask()
    : m_priority(80)
{
    YYS_LOG_INFO("YYSSignInTask constructed");
}

bool YYSSignInTask::_run()
{
    YYS_LOG_INFO("========== YYSSignInTask started ==========");

    if (!check_resource()) {
        YYS_LOG_ERROR("Resource check failed");
        return false;
    }

    // Find and click sign-in button
    YYSWaitFlow wait_signin("I_SIGN_IN", DEFAULT_TIMEOUT_MS);
    wait_signin.init(m_ctx);
    if (!wait_signin.run()) {
        YYS_LOG_INFO("Sign-in button not found, may already be signed in");
        return true;
    }

    YYS_LOG_INFO("Sign-in button found, clicking...");
    YYSRetryFlow click_signin(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_SIGN_IN"),
        3);
    click_signin.init(m_ctx);
    click_signin.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Handle reward popup
    YYSWaitFlow wait_reward("I_SIGN_IN_REWARD", 5000);
    wait_reward.init(m_ctx);
    if (wait_reward.run()) {
        YYS_LOG_INFO("Sign-in reward detected, clicking...");
        int attempts = 0;
        while (attempts < 5) {
            YYSWaitFlow check("I_SIGN_IN_REWARD", 2000);
            check.init(m_ctx);
            if (!check.run()) break;

            YYSActionFlow click(ActionType::Click, 0, 0, "I_SIGN_IN_REWARD");
            click.init(m_ctx);
            click.run();
            ++attempts;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }

    // Close sign-in page
    YYSWaitFlow wait_close("I_SIGN_IN_CLOSE", 3000);
    wait_close.init(m_ctx);
    if (wait_close.run()) {
        YYSActionFlow click_close(ActionType::Click, 0, 0, "I_SIGN_IN_CLOSE");
        click_close.init(m_ctx);
        click_close.run();
    }

    YYS_LOG_INFO("========== YYSSignInTask completed ==========");
    return true;
}

} // namespace asst::yys
