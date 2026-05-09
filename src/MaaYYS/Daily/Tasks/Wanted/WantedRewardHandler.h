#pragma once

#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class WantedRewardHandler {
public:
    explicit WantedRewardHandler(std::shared_ptr<YYSContext> ctx);

    bool wait_and_handle();

private:
    std::shared_ptr<YYSContext> m_ctx;
    static constexpr int BATTLE_TIMEOUT_MS = 120000;
    static constexpr int REWARD_TIMEOUT_MS = 2000;
};

} // namespace asst::yys
