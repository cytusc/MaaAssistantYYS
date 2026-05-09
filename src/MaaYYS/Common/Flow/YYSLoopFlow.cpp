#include "YYSLoopFlow.h"

namespace asst::yys {

YYSLoopFlow::YYSLoopFlow(int max_loop)
    : YYSFlowBase("LoopFlow")
    , m_max_loop(max_loop)
{}

bool YYSLoopFlow::run() {
    int loop_count = 0;
    
    while (m_max_loop < 0 || loop_count < m_max_loop) {
        if (!m_next->run()) {
            return false;
        }
        ++loop_count;
    }
    return true;
}

} // namespace asst::yys
