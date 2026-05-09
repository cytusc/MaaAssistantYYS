#pragma once

#include <functional>

#include "Base/IActionExecutor.h"

namespace asst::yys {

class MaaControllerActionExecutor : public IActionExecutor {
public:
    using ScreencapFn = std::function<bool()>;
    using ClickFn = std::function<bool(Point)>;
    using SwipeFn = std::function<bool(Point, Point)>;
    using LongClickFn = std::function<bool(Point, int)>;

    MaaControllerActionExecutor() = default;
    MaaControllerActionExecutor(ScreencapFn screencap, ClickFn click, SwipeFn swipe, LongClickFn long_click = {});

    void bind_screencap(ScreencapFn screencap);
    void bind_click(ClickFn click);
    void bind_swipe(SwipeFn swipe);
    void bind_long_click(LongClickFn long_click);

    bool screencap() override;
    bool click(Point point) override;
    bool swipe(Point from, Point to) override;
    bool long_click(Point point, int duration_ms = 800) override;

private:
    ScreencapFn m_screencap;
    ClickFn m_click;
    SwipeFn m_swipe;
    LongClickFn m_long_click;
};

} // namespace asst::yys
