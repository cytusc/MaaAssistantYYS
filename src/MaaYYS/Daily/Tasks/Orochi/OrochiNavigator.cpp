#include "OrochiNavigator.h"

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

OrochiNavigator::OrochiNavigator(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool OrochiNavigator::enter()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Entering orochi interface...");

    YYSWaitFlow wait_flow("I_OROCHI", DEFAULT_TIMEOUT_MS);
    wait_flow.init(m_ctx);
    if (!wait_flow.run()) {
        YYS_LOG_ERROR("Orochi entrance not found");
        return false;
    }

    YYSRetryFlow click_flow(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_OROCHI"),
        3);
    click_flow.init(m_ctx);
    if (!click_flow.run()) {
        YYS_LOG_ERROR("Failed to click orochi entrance");
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    YYS_LOG_INFO("Successfully entered orochi interface");
    return true;
}

} // namespace asst::yys
