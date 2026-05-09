#include "MaaControllerActionExecutor.h"

namespace asst::yys {

MaaControllerActionExecutor::MaaControllerActionExecutor(
    ScreencapFn screencap,
    ClickFn click,
    SwipeFn swipe,
    LongClickFn long_click)
    : m_screencap(std::move(screencap))
    , m_click(std::move(click))
    , m_swipe(std::move(swipe))
    , m_long_click(std::move(long_click))
{}

void MaaControllerActionExecutor::bind_screencap(ScreencapFn screencap)
{
    m_screencap = std::move(screencap);
}

void MaaControllerActionExecutor::bind_click(ClickFn click)
{
    m_click = std::move(click);
}

void MaaControllerActionExecutor::bind_swipe(SwipeFn swipe)
{
    m_swipe = std::move(swipe);
}

void MaaControllerActionExecutor::bind_long_click(LongClickFn long_click)
{
    m_long_click = std::move(long_click);
}

bool MaaControllerActionExecutor::screencap()
{
    return m_screencap ? m_screencap() : false;
}

bool MaaControllerActionExecutor::click(Point point)
{
    return m_click ? m_click(point) : false;
}

bool MaaControllerActionExecutor::swipe(Point from, Point to)
{
    return m_swipe ? m_swipe(from, to) : false;
}

bool MaaControllerActionExecutor::long_click(Point point, int duration_ms)
{
    if (m_long_click) {
        return m_long_click(point, duration_ms);
    }
    return IActionExecutor::long_click(point, duration_ms);
}

} // namespace asst::yys
