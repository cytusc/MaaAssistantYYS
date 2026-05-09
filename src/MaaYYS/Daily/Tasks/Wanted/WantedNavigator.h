#pragma once

#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class WantedNavigator {
public:
    explicit WantedNavigator(std::shared_ptr<YYSContext> ctx);

    bool enter();

private:
    std::shared_ptr<YYSContext> m_ctx;
    static constexpr int DEFAULT_TIMEOUT_MS = 10000;
};

} // namespace asst::yys
