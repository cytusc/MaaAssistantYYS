#include "DeviceConnector.h"
#include "AdbDirectBridge.h"
#include "Common/Controller/BridgedActionExecutor.h"
#include "Common/Vision/MaaTemplateResolver.h"
#include "Common/Resource/YYSResourceLoader.h"
#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

std::shared_ptr<MaaCoreBridge> DeviceConnector::create_bridge(const DeviceConnectionConfig& config)
{
    auto bridge = std::make_shared<AdbDirectBridge>();

    MaaCoreConnectionParams params;
    params.adb_path = config.adb_path;
    params.address = config.address;

    if (!bridge->connect(params)) {
        YYS_LOG_ERROR("Failed to connect to device: %s", config.address.c_str());
        return nullptr;
    }

    return bridge;
}

std::shared_ptr<YYSContext> DeviceConnector::connect(const DeviceConnectionConfig& config)
{
    auto bridge = create_bridge(config);
    if (!bridge) return nullptr;

    auto executor = std::make_shared<BridgedActionExecutor>(
        std::static_pointer_cast<MaaCoreBridge>(bridge));

    YYSResourceLoader loader;
    loader.load(config.resource_path);

    auto resolver = std::make_shared<MaaTemplateResolver>(loader);

    auto ctx = std::make_shared<YYSContext>();
    ctx->set_executor(executor);
    ctx->set_resolver(resolver);

    YYS_LOG_INFO("Device context created: %s, templates: %zu",
                 bridge->uuid().c_str(), loader.templates().size());
    return ctx;
}

} // namespace asst::yys
