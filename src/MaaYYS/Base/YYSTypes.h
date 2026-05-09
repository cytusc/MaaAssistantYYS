#pragma once

namespace asst::yys {

struct Point {
    int x = 0;
    int y = 0;

    bool empty() const noexcept
    {
        return x == 0 && y == 0;
    }
};

struct Rect {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    bool empty() const noexcept
    {
        return width <= 0 || height <= 0;
    }
};

} // namespace asst::yys
