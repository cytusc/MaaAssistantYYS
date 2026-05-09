#pragma once

#include <vector>

#include "Base/IActionExecutor.h"

namespace asst::yys {

enum class RecordedActionType {
    Screencap,
    Click,
    Swipe,
    LongClick,
};

struct RecordedAction {
    RecordedActionType type = RecordedActionType::Screencap;
    Point from;
    Point to;
    int duration_ms = 0;
};

class RecordingActionExecutor : public IActionExecutor {
public:
    bool screencap() override;
    bool click(Point point) override;
    bool swipe(Point from, Point to) override;
    bool long_click(Point point, int duration_ms = 800) override;

    const std::vector<RecordedAction>& actions() const noexcept { return m_actions; }
    void clear() { m_actions.clear(); }

private:
    std::vector<RecordedAction> m_actions;
};

} // namespace asst::yys
