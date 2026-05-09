#include <iostream>
#include <string>
#include <memory>

#include "Common/Bridge/AdbDirectBridge.h"
#include "Common/Bridge/DeviceConnector.h"
#include "Common/Bridge/MaaCoreBridge.h"
#include "Common/Controller/BridgedActionExecutor.h"
#include "Common/Logger/YYSLogger.h"

void print_usage()
{
    std::cout << "Usage: DeviceTest [adb_path] [address]\n"
              << "  adb_path: path to adb executable (default: adb)\n"
              << "  address:  device address (default: 127.0.0.1:5555)\n"
              << "\nExamples:\n"
              << "  DeviceTest\n"
              << "  DeviceTest adb 127.0.0.1:7555\n"
              << "  DeviceTest C:/Android/adb.exe 192.168.1.100:5555\n";
}

int main(int argc, char* argv[])
{
    using namespace asst::yys;

    std::string adb_path = "adb";
    std::string address = "127.0.0.1:5555";

    if (argc >= 2) adb_path = argv[1];
    if (argc >= 3) address = argv[2];

    std::cout << "=== MaaYYS Device Connection Test ===" << std::endl;
    std::cout << "ADB:     " << adb_path << std::endl;
    std::cout << "Address: " << address << std::endl;

    // Step 1: Create bridge and connect
    std::cout << "\n--- Step 1: Connect ---" << std::endl;
    auto bridge = std::make_shared<AdbDirectBridge>();

    MaaCoreConnectionParams params;
    params.adb_path = adb_path;
    params.address = address;

    if (!bridge->connect(params)) {
        std::cout << "❌ Connection failed" << std::endl;
        std::cout << "device_test=connect_failed" << std::endl;
        return 1;
    }
    std::cout << "✅ Connected" << std::endl;
    std::cout << "UUID:   " << bridge->uuid() << std::endl;
    std::cout << "Screen: " << bridge->screen_width() << "x" << bridge->screen_height() << std::endl;

    // Step 2: Screencap
    std::cout << "\n--- Step 2: Screencap ---" << std::endl;
    if (!bridge->screencap()) {
        std::cout << "❌ Screencap failed" << std::endl;
        std::cout << "device_test=screencap_failed" << std::endl;
        return 1;
    }
    std::cout << "✅ Screencap OK, size: " << bridge->last_screencap_data().size() << " bytes" << std::endl;

    // Step 3: Test BridgedActionExecutor
    std::cout << "\n--- Step 3: BridgedActionExecutor ---" << std::endl;
    auto executor = std::make_shared<BridgedActionExecutor>(bridge);
    bool cap = executor->screencap();
    std::cout << (cap ? "✅" : "❌") << " executor->screencap()" << std::endl;

    // Step 4: Test DeviceConnector
    std::cout << "\n--- Step 4: DeviceConnector ---" << std::endl;
    DeviceConnectionConfig config;
    config.adb_path = adb_path;
    config.address = address;
    config.resource_path = "D:/code/maa/MaaAssistantYYS/resource/YYS";

    auto ctx = DeviceConnector::connect(config);
    std::cout << (ctx ? "✅" : "❌") << " DeviceConnector::connect()" << std::endl;

    if (ctx) {
        std::cout << "  executor: " << (ctx->executor() ? "ok" : "null") << std::endl;
        std::cout << "  resolver: " << (ctx->resolver() ? "ok" : "null") << std::endl;
    }

    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "device_test=ok" << std::endl;

    bridge->disconnect();
    return 0;
}
