#include "YYSActionFlow.h"

#include "Base/IActionExecutor.h"
#include "Base/ITemplateResolver.h"
#include "Base/YYSTypes.h"
#include "Base/YYSContext.h"
#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

YYSActionFlow::YYSActionFlow(ActionType type, int x, int y, const std::string& target)
    : YYSFlowBase("ActionFlow")
    , m_type(type)
    , m_x(x)
    , m_y(y)
    , m_x2(x)
    , m_y2(y)
    , m_target(target)
{}

YYSActionFlow::YYSActionFlow(ActionType type, int x, int y, int x2, int y2, const std::string& target)
    : YYSFlowBase("ActionFlow")
    , m_type(type)
    , m_x(x)
    , m_y(y)
    , m_x2(x2)
    , m_y2(y2)
    , m_target(target)
{}

bool YYSActionFlow::run()
{
    if (!m_ctx || !m_ctx->executor()) {
        YYS_LOG_ERROR("ActionFlow: context not available");
        return false;
    }

    if (!m_target.empty() && m_ctx->resolver()) {
        const auto rect = m_ctx->resolver()->find_template(m_target);
        if (rect && !rect->empty()) {
            m_x = rect->x + rect->width / 2;
            m_y = rect->y + rect->height / 2;
            if (m_type == ActionType::Swipe) {
                m_x2 = m_x;
                m_y2 = m_y;
            }
        }
    }

    switch (m_type) {
    case ActionType::Click:
        YYS_LOG_DEBUG("ActionFlow: click at (%d, %d)", m_x, m_y);
        return m_ctx->executor()->click(Point { m_x, m_y });
    case ActionType::LongClick:
        YYS_LOG_DEBUG("ActionFlow: long_click at (%d, %d)", m_x, m_y);
        return m_ctx->executor()->long_click(Point { m_x, m_y });
    case ActionType::Swipe:
        YYS_LOG_DEBUG("ActionFlow: swipe (%d,%d) -> (%d,%d)", m_x, m_y, m_x2, m_y2);
        return m_ctx->executor()->swipe(Point { m_x, m_y }, Point { m_x2, m_y2 });
    case ActionType::Text:
    default:
        YYS_LOG_ERROR("ActionFlow: unsupported action type");
        return false;
    }
}

} // namespace asst::yys
