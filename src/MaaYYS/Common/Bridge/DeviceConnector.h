#pragma once

#include <memory>
#include <string>

#include "Base/YYSContext.h"
#include "Common/Bridge/MaaCoreBridge.h"

namespace asst::yys {

struct DeviceConnectionConfig {
    std::string adb_path = "adb";
    std::string address = "127.0.0.1:5555";
    std::string resource_path = "resource/YYS";
};

class DeviceConnector {
public:
    static std::shared_ptr<YYSContext> connect(const DeviceConnectionConfig& config);
    static std::shared_ptr<MaaCoreBridge> create_bridge(const DeviceConnectionConfig& config);
};

} // namespace asst::yys
