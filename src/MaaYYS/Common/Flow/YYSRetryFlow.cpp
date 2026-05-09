#include "YYSRetryFlow.h"

#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

YYSRetryFlow::YYSRetryFlow(std::unique_ptr<YYSFlowBase> flow, int max_retry)
    : YYSFlowBase("RetryFlow")
    , m_flow(std::move(flow))
    , m_max_retry(max_retry)
{}

bool YYSRetryFlow::run()
{
    if (!m_flow) {
        YYS_LOG_ERROR("RetryFlow: no inner flow");
        return false;
    }

    if (m_ctx) {
        m_flow->init(m_ctx);
    }

    for (int i = 0; i < m_max_retry; ++i) {
        YYS_LOG_DEBUG("RetryFlow: attempt %d/%d", i + 1, m_max_retry);
        if (m_flow->run()) {
            YYS_LOG_DEBUG("RetryFlow: succeeded on attempt %d", i + 1);
            return true;
        }
    }

    YYS_LOG_WARN("RetryFlow: all %d attempts failed", m_max_retry);
    return false;
}

} // namespace asst::yys
