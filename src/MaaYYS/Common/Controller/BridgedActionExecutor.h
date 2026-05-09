#pragma once

#include "Base/IActionExecutor.h"
#include "Common/Bridge/MaaCoreBridge.h"
#include <memory>

namespace asst::yys {

class BridgedActionExecutor : public IActionExecutor {
public:
    explicit BridgedActionExecutor(std::shared_ptr<MaaCoreBridge> bridge);

    bool screencap() override;
    bool click(Point point) override;
    bool swipe(Point from, Point to) override;
    bool long_click(Point point, int duration_ms = 800) override;

    std::shared_ptr<MaaCoreBridge> bridge() const { return m_bridge; }

private:
    std::shared_ptr<MaaCoreBridge> m_bridge;
};

} // namespace asst::yys
