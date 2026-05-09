#include "YYSWaitFlow.h"

#include <chrono>
#include <thread>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSContext.h"

namespace asst::yys {

YYSWaitFlow::YYSWaitFlow(const std::string& target, int timeout_ms)
    : YYSFlowBase("WaitFlow")
    , m_target(target)
    , m_timeout_ms(timeout_ms)
{}

bool YYSWaitFlow::run()
{
    if (!m_ctx || !m_ctx->resolver() || !m_ctx->executor()) {
        return false;
    }

    const auto start = std::chrono::steady_clock::now();

    while (true) {
        const auto rect = m_ctx->resolver()->find_template(m_target);
        if (rect && !rect->empty()) {
            return true;
        }

        const auto elapsed = std::chrono::steady_clock::now() - start;
        if (elapsed > std::chrono::milliseconds(m_timeout_ms)) {
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        m_ctx->executor()->screencap();
    }
}

} // namespace asst::yys
