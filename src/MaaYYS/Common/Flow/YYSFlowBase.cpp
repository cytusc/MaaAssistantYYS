#include "YYSFlowBase.h"

namespace asst::yys {

bool YYSFlowBase::init(std::shared_ptr<YYSContext> ctx)
{
    m_ctx = std::move(ctx);
    if (m_next) {
        m_next->init(m_ctx);
    }
    return true;
}

void YYSFlowBase::set_next(std::unique_ptr<YYSFlowBase> next)
{
    if (next && m_ctx) {
        next->init(m_ctx);
    }
    m_next = std::move(next);
}

} // namespace asst::yys
