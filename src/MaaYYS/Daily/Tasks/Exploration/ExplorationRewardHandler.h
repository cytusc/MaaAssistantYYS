#pragma once

#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class ExplorationRewardHandler {
public:
    explicit ExplorationRewardHandler(std::shared_ptr<YYSContext> ctx);

    bool handle();

private:
    std::shared_ptr<YYSContext> m_ctx;
    static constexpr int DEFAULT_TIMEOUT_MS = 2000;
};

} // namespace asst::yys
