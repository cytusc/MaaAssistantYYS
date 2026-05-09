#include "RecordingActionExecutor.h"

namespace asst::yys {

bool RecordingActionExecutor::screencap()
{
    m_actions.push_back(RecordedAction { .type = RecordedActionType::Screencap });
    return true;
}

bool RecordingActionExecutor::click(Point point)
{
    m_actions.push_back(RecordedAction { .type = RecordedActionType::Click, .from = point, .to = point });
    return true;
}

bool RecordingActionExecutor::swipe(Point from, Point to)
{
    m_actions.push_back(RecordedAction { .type = RecordedActionType::Swipe, .from = from, .to = to });
    return true;
}

bool RecordingActionExecutor::long_click(Point point, int duration_ms)
{
    m_actions.push_back(RecordedAction {
        .type = RecordedActionType::LongClick,
        .from = point,
        .to = point,
        .duration_ms = duration_ms,
    });
    return true;
}

} // namespace asst::yys
