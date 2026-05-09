#include "MaaCoreBridge.h"
#include "AdbDirectBridge.h"

namespace asst::yys {

std::unique_ptr<MaaCoreBridge> MaaCoreBridgeFactory::create()
{
    return std::make_unique<AdbDirectBridge>();
}

} // namespace asst::yys
