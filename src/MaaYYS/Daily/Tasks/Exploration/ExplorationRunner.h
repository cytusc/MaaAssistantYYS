#pragma once

#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class ExplorationRunner {
public:
    explicit ExplorationRunner(std::shared_ptr<YYSContext> ctx);

    void set_max_nodes(int max_nodes) { m_max_nodes = max_nodes; }

    bool enter_chapter();
    bool explore_loop();

private:
    bool try_click_node();
    bool wait_battle_end();

    std::shared_ptr<YYSContext> m_ctx;
    int m_max_nodes = 20;
    int m_nodes_cleared = 0;
    static constexpr int BATTLE_TIMEOUT_MS = 120000;
    static constexpr int NODE_DETECT_TIMEOUT_MS = 5000;
};

} // namespace asst::yys
