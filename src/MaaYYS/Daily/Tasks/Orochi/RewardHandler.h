#pragma once

#include <string>
#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class RewardHandler {
public:
    explicit RewardHandler(std::shared_ptr<YYSContext> ctx);
    ~RewardHandler() = default;

    // 处理奖励弹窗
    bool handle();

private:
    bool click_reward_button();

    std::shared_ptr<YYSContext> m_ctx;
    static constexpr int DEFAULT_TIMEOUT_MS = 2000;
};

} // namespace asst::yys
