#include "YYSRetryFlow.h"

namespace asst::yys {

YYSRetryFlow::YYSRetryFlow(std::unique_ptr<YYSFlowBase> flow, int max_retry)
    : YYSFlowBase("RetryFlow")
    , m_flow(std::move(flow))
    , m_max_retry(max_retry)
{}

bool YYSRetryFlow::run()
{
    if (!m_flow) {
        return false;
    }

    if (m_ctx) {
        m_flow->init(m_ctx);
    }

    for (int i = 0; i < m_max_retry; ++i) {
        if (m_flow->run()) {
            return true;
        }
    }

    return false;
}

} // namespace asst::yys
