#include "BridgedActionExecutor.h"
#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

BridgedActionExecutor::BridgedActionExecutor(std::shared_ptr<MaaCoreBridge> bridge)
    : m_bridge(std::move(bridge))
{
}

bool BridgedActionExecutor::screencap()
{
    if (!m_bridge || !m_bridge->connected()) {
        YYS_LOG_ERROR("Bridge not connected");
        return false;
    }
    return m_bridge->screencap();
}

bool BridgedActionExecutor::click(Point point)
{
    if (!m_bridge || !m_bridge->connected()) {
        YYS_LOG_ERROR("Bridge not connected");
        return false;
    }
    return m_bridge->click(point.x, point.y);
}

bool BridgedActionExecutor::swipe(Point from, Point to)
{
    if (!m_bridge || !m_bridge->connected()) {
        YYS_LOG_ERROR("Bridge not connected");
        return false;
    }
    return m_bridge->swipe(from.x, from.y, to.x, to.y);
}

bool BridgedActionExecutor::long_click(Point point, int duration_ms)
{
    if (!m_bridge || !m_bridge->connected()) {
        YYS_LOG_ERROR("Bridge not connected");
        return false;
    }
    return m_bridge->long_click(point.x, point.y, duration_ms);
}

} // namespace asst::yys
