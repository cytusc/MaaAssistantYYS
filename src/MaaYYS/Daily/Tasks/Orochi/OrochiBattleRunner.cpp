#include "OrochiBattleRunner.h"

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

OrochiBattleRunner::OrochiBattleRunner(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool OrochiBattleRunner::start()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Starting battle...");

    YYSRetryFlow click_flow(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_OROCHI_FIRE"),
        3);
    click_flow.init(m_ctx);
    if (!click_flow.run()) {
        YYS_LOG_ERROR("Failed to start battle");
        return false;
    }

    YYS_LOG_INFO("Battle started successfully");
    return true;
}

bool OrochiBattleRunner::wait_for_end()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Waiting for battle to end...");

    YYSWaitFlow wait_flow("I_REWARD", BATTLE_TIMEOUT_MS);
    wait_flow.init(m_ctx);
    if (!wait_flow.run()) {
        YYS_LOG_ERROR("Battle reward screen not found (timeout)");
        return false;
    }

    YYS_LOG_INFO("Battle ended, reward screen detected");
    return true;
}

} // namespace asst::yys
