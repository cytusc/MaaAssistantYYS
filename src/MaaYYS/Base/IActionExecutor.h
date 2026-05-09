#pragma once

#include "YYSTypes.h"

namespace asst::yys {

class IActionExecutor {
public:
    virtual ~IActionExecutor() = default;

    virtual bool screencap() = 0;
    virtual bool click(Point point) = 0;
    virtual bool swipe(Point from, Point to) = 0;

    virtual bool long_click(Point point, int duration_ms = 800)
    {
        (void)duration_ms;
        return click(point);
    }
};

} // namespace asst::yys
