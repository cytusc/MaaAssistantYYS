#pragma once

#include <string>
#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class OrochiNavigator {
public:
    explicit OrochiNavigator(std::shared_ptr<YYSContext> ctx);
    ~OrochiNavigator() = default;

    // 进入御魂界面
    bool enter();

private:
    bool wait_for_entrance();
    bool click_entrance();

    std::shared_ptr<YYSContext> m_ctx;
    static constexpr int DEFAULT_TIMEOUT_MS = 10000;
};

} // namespace asst::yys
