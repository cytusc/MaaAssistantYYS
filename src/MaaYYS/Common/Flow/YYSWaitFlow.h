#pragma once
#include "YYSFlowBase.h"

#include "Base/YYSTypes.h"

#include <optional>

namespace asst::yys {

class YYSWaitFlow : public YYSFlowBase {
public:
    YYSWaitFlow(const std::string& target, int timeout_ms = 30000);
    bool run() override;

    const std::optional<Rect>& found_rect() const { return m_found_rect; }

private:
    std::string m_target;
    int m_timeout_ms;
    std::optional<Rect> m_found_rect;
};

} // namespace asst::yys
