#include "YYSActionFlow.h"

#include "Base/IActionExecutor.h"
#include "Base/ITemplateResolver.h"
#include "Base/YYSContext.h"

namespace asst::yys {

YYSActionFlow::YYSActionFlow(ActionType type, int x, int y, const std::string& target)
    : YYSFlowBase("ActionFlow")
    , m_type(type)
    , m_x(x)
    , m_y(y)
    , m_target(target)
{}

bool YYSActionFlow::run()
{
    if (!m_ctx || !m_ctx->executor()) {
        return false;
    }

    if (!m_target.empty() && m_ctx->resolver()) {
        const auto rect = m_ctx->resolver()->find_template(m_target);
        if (rect && !rect->empty()) {
            m_x = rect->x + rect->width / 2;
            m_y = rect->y + rect->height / 2;
        }
    }

    switch (m_type) {
    case ActionType::Click:
        return m_ctx->executor()->click(Point { m_x, m_y });
    case ActionType::LongClick:
        return m_ctx->executor()->long_click(Point { m_x, m_y });
    case ActionType::Swipe:
        return m_ctx->executor()->swipe(Point { m_x, m_y }, Point { m_x, m_y });
    case ActionType::Text:
    default:
        return false;
    }
}

} // namespace asst::yys
