#include "YYSLoopFlow.h"

#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

YYSLoopFlow::YYSLoopFlow(int max_loop)
    : YYSFlowBase("LoopFlow")
    , m_max_loop(max_loop)
{}

bool YYSLoopFlow::run()
{
    if (!m_next) {
        YYS_LOG_ERROR("LoopFlow: no next flow");
        return false;
    }

    int loop_count = 0;

    while (m_max_loop < 0 || loop_count < m_max_loop) {
        YYS_LOG_DEBUG("LoopFlow: iteration %d", loop_count + 1);
        if (!m_next->run()) {
            YYS_LOG_DEBUG("LoopFlow: next flow failed, stopping loop");
            break;
        }
        ++loop_count;
    }

    YYS_LOG_DEBUG("LoopFlow: completed %d iterations", loop_count);
    return true;
}

} // namespace asst::yys
