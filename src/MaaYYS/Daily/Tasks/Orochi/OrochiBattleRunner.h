#pragma once

#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class OrochiBattleRunner {
public:
    explicit OrochiBattleRunner(std::shared_ptr<YYSContext> ctx);

    bool start();
    bool wait_for_end();

private:
    std::shared_ptr<YYSContext> m_ctx;
    static constexpr int BATTLE_TIMEOUT_MS = 120000;
};

} // namespace asst::yys
