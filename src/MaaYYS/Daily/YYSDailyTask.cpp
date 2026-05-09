#include "YYSDailyTask.h"
#include "Base/YYSContext.h"

namespace asst::yys {

bool YYSDailyTask::run()
{
    if (!check_enable()) {
        return false;
    }

    const bool result = _run();
    if (result) {
        return on_executed();
    }
    return false;
}

bool YYSDailyTask::check_enable() const
{
    return static_cast<bool>(m_ctx);
}

bool YYSDailyTask::check_resource() const
{
    return m_ctx && m_ctx->resolver() && m_ctx->executor();
}

bool YYSDailyTask::on_executed()
{
    return true;
}

} // namespace asst::yys
