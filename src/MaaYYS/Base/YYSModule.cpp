#include "YYSModule.h"

namespace asst::yys {

bool YYSModule::init(std::shared_ptr<YYSContext> ctx)
{
    m_ctx = ctx;
    return true;
}

} // namespace asst::yys
