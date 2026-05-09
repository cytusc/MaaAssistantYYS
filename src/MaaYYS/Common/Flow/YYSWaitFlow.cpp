#include "YYSWaitFlow.h"

#include <chrono>
#include <thread>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSContext.h"
#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

YYSWaitFlow::YYSWaitFlow(const std::string& target, int timeout_ms)
    : YYSFlowBase("WaitFlow")
    , m_target(target)
    , m_timeout_ms(timeout_ms)
{}

bool YYSWaitFlow::run()
{
    if (!m_ctx || !m_ctx->resolver() || !m_ctx->executor()) {
        YYS_LOG_ERROR("WaitFlow: context not available");
        return false;
    }

    YYS_LOG_DEBUG("WaitFlow: waiting for %s, timeout=%dms", m_target.c_str(), m_timeout_ms);

    const auto start = std::chrono::steady_clock::now();
    constexpr int poll_interval = 500;

    while (true) {
        const auto rect = m_ctx->resolver()->find_template(m_target);
        if (rect && !rect->empty()) {
            m_found_rect = rect;
            YYS_LOG_DEBUG("WaitFlow: found %s at (%d,%d)", m_target.c_str(), rect->x, rect->y);
            return true;
        }

        const auto elapsed = std::chrono::steady_clock::now() - start;
        if (elapsed > std::chrono::milliseconds(m_timeout_ms)) {
            YYS_LOG_WARN("WaitFlow: timeout waiting for %s", m_target.c_str());
            m_found_rect = std::nullopt;
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval));
        m_ctx->executor()->screencap();
    }
}

} // namespace asst::yys
