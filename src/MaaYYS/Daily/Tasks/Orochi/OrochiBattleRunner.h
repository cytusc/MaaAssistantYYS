#pragma once

#include <string>
#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class OrochiBattleRunner {
public:
    explicit OrochiBattleRunner(std::shared_ptr<YYSContext> ctx);
    ~OrochiBattleRunner() = default;

    // 开始战斗
    bool start();
    // 等待战斗结束
    bool wait_for_end();

private:
    bool click_start_button();
    bool wait_for_reward_screen();

    std::shared_ptr<YYSContext> m_ctx;
    static constexpr int DEFAULT_TIMEOUT_MS = 10000;
    static constexpr int BATTLE_TIMEOUT_MS = 120000;
};

} // namespace asst::yys
