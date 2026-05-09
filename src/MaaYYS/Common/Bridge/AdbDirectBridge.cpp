#include "AdbDirectBridge.h"
#include "Common/Logger/YYSLogger.h"

#include <array>
#include <cstdio>
#include <sstream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

namespace asst::yys {

AdbDirectBridge::AdbDirectBridge() = default;

AdbDirectBridge::~AdbDirectBridge()
{
    disconnect();
}

std::string AdbDirectBridge::adb_command(const std::string& args) const
{
    std::string cmd = "\"" + m_adb_path + "\"";
    if (!m_address.empty()) {
        cmd += " -s " + m_address;
    }
    cmd += " " + args;
    return cmd;
}

bool AdbDirectBridge::exec_adb(const std::string& args, std::string& output, int timeout_ms) const
{
    std::string cmd = adb_command(args) + " 2>/dev/null";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        YYS_LOG_ERROR("Failed to execute: %s", cmd.c_str());
        return false;
    }

    std::array<char, 4096> buffer;
    output.clear();
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        output += buffer.data();
    }

    int status = pclose(pipe);
    return status == 0;
}

bool AdbDirectBridge::exec_adb_silent(const std::string& args, int timeout_ms) const
{
    std::string output;
    return exec_adb(args, output, timeout_ms);
}

bool AdbDirectBridge::connect(const MaaCoreConnectionParams& params)
{
    std::lock_guard lock(m_mutex);

    m_adb_path = params.adb_path;
    m_address = params.address;

    if (m_adb_path.empty()) {
        m_adb_path = "adb";
    }

    YYS_LOG_INFO("Connecting to device: %s via %s", m_address.c_str(), m_adb_path.c_str());

    std::string output;
    if (!exec_adb("connect " + m_address, output)) {
        YYS_LOG_ERROR("adb connect failed");
        return false;
    }

    if (output.find("connected") == std::string::npos &&
        output.find("already") == std::string::npos) {
        YYS_LOG_ERROR("Connection failed: %s", output.c_str());
        return false;
    }

    // Get device serial / UUID
    if (!exec_adb("get-serialno", output)) {
        m_uuid = m_address;
    } else {
        m_uuid = output;
        while (!m_uuid.empty() && (m_uuid.back() == '\n' || m_uuid.back() == '\r')) {
            m_uuid.pop_back();
        }
    }

    if (!detect_screen_size()) {
        YYS_LOG_WARN("Failed to detect screen size, using default 1280x720");
    }

    m_connected = true;
    YYS_LOG_INFO("Connected to %s, UUID: %s, Screen: %dx%d",
                 m_address.c_str(), m_uuid.c_str(), m_width, m_height);
    return true;
}

void AdbDirectBridge::disconnect()
{
    std::lock_guard lock(m_mutex);
    if (m_connected) {
        exec_adb_silent("disconnect " + m_address);
        m_connected = false;
        YYS_LOG_INFO("Disconnected from %s", m_address.c_str());
    }
}

bool AdbDirectBridge::detect_screen_size()
{
    std::string output;
    if (!exec_adb("shell wm size", output)) {
        return false;
    }

    // Parse "Physical size: 1280x720"
    auto pos = output.find("size:");
    if (pos == std::string::npos) return false;

    int w = 0, h = 0;
    if (sscanf(output.c_str() + pos + 5, " %dx%d", &w, &h) == 2 && w > 0 && h > 0) {
        m_width = w;
        m_height = h;
        return true;
    }
    return false;
}

bool AdbDirectBridge::screencap()
{
    std::lock_guard lock(m_mutex);

    std::string cmd = adb_command("exec-out screencap -p") + " 2>/dev/null";
    FILE* pipe = popen(cmd.c_str(), "rb");
    if (!pipe) {
        YYS_LOG_ERROR("Failed to execute screencap");
        return false;
    }

    m_screencap_data.clear();
    std::array<uint8_t, 65536> buffer;
    size_t bytes_read;
    while ((bytes_read = fread(buffer.data(), 1, buffer.size(), pipe)) > 0) {
        m_screencap_data.insert(m_screencap_data.end(),
                                buffer.begin(), buffer.begin() + bytes_read);
    }

    int status = pclose(pipe);

    if (m_screencap_data.empty()) {
        YYS_LOG_ERROR("Screencap returned empty data");
        return false;
    }

    // Notify callback with raw data
    if (m_screencap_cb) {
        m_screencap_cb(m_screencap_data.data(),
                       static_cast<int>(m_screencap_data.size()), 0, 0);
    }

    YYS_LOG_DEBUG("Screencap captured: %zu bytes", m_screencap_data.size());
    return true;
}

bool AdbDirectBridge::click(int x, int y)
{
    std::lock_guard lock(m_mutex);
    YYS_LOG_DEBUG("Click: (%d, %d)", x, y);
    std::string cmd = "shell input tap " + std::to_string(x) + " " + std::to_string(y);
    return exec_adb_silent(cmd);
}

bool AdbDirectBridge::swipe(int x1, int y1, int x2, int y2, int duration)
{
    std::lock_guard lock(m_mutex);
    YYS_LOG_DEBUG("Swipe: (%d,%d) -> (%d,%d) duration=%d", x1, y1, x2, y2, duration);
    std::string cmd = "shell input swipe "
        + std::to_string(x1) + " " + std::to_string(y1) + " "
        + std::to_string(x2) + " " + std::to_string(y2) + " "
        + std::to_string(duration);
    return exec_adb_silent(cmd);
}

bool AdbDirectBridge::long_click(int x, int y, int duration)
{
    return swipe(x, y, x, y, duration);
}

bool AdbDirectBridge::press_key(int key)
{
    std::lock_guard lock(m_mutex);
    std::string cmd = "shell input keyevent " + std::to_string(key);
    return exec_adb_silent(cmd);
}

bool AdbDirectBridge::input_text(const std::string& text)
{
    std::lock_guard lock(m_mutex);
    std::string cmd = "shell input text \"" + text + "\"";
    return exec_adb_silent(cmd);
}

bool AdbDirectBridge::start_game(const std::string& client_type)
{
    std::lock_guard lock(m_mutex);
    std::string package = "com.netease.onmyoji";
    if (client_type == "bili") {
        package = "com.netease.onmyoji.bili";
    }
    std::string cmd = "shell am start -n " + package + "/.LobbyActivity";
    YYS_LOG_INFO("Starting game: %s", package.c_str());
    return exec_adb_silent(cmd);
}

bool AdbDirectBridge::stop_game(const std::string& client_type)
{
    std::lock_guard lock(m_mutex);
    std::string package = "com.netease.onmyoji";
    if (client_type == "bili") {
        package = "com.netease.onmyoji.bili";
    }
    std::string cmd = "shell am force-stop " + package;
    YYS_LOG_INFO("Stopping game: %s", package.c_str());
    return exec_adb_silent(cmd);
}

} // namespace asst::yys
