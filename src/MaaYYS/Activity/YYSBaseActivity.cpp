#include "YYSBaseActivity.h"

namespace asst::yys {

bool YYSBaseActivity::init(std::shared_ptr<YYSContext> ctx)
{
    return YYSModule::init(std::move(ctx));
}

bool YYSBaseActivity::run()
{
    if (!on_start()) {
        return false;
    }

    const bool result = _run();
    on_end();
    return result;
}

bool YYSBaseActivity::on_start()
{
    return true;
}

bool YYSBaseActivity::on_end()
{
    return true;
}

} // namespace asst::yys
