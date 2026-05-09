#pragma once

#include "MaaCoreBridge.h"
#include <string>
#include <vector>
#include <mutex>

namespace asst::yys {

class AdbDirectBridge : public MaaCoreBridge {
public:
    AdbDirectBridge();
    ~AdbDirectBridge() override;

    bool connect(const MaaCoreConnectionParams& params) override;
    bool connected() const override { return m_connected; }
    void disconnect() override;

    bool screencap() override;
    bool click(int x, int y) override;
    bool swipe(int x1, int y1, int x2, int y2, int duration = 300) override;
    bool long_click(int x, int y, int duration = 800) override;
    bool press_key(int key) override;
    bool input_text(const std::string& text) override;

    bool start_game(const std::string& client_type) override;
    bool stop_game(const std::string& client_type) override;

    int screen_width() const override { return m_width; }
    int screen_height() const override { return m_height; }
    std::string uuid() const override { return m_uuid; }

    const std::vector<uint8_t>& last_screencap_data() const { return m_screencap_data; }

private:
    std::string adb_command(const std::string& args) const;
    bool exec_adb(const std::string& args, std::string& output, int timeout_ms = 10000) const;
    bool exec_adb_silent(const std::string& args, int timeout_ms = 10000) const;
    bool detect_screen_size();

    std::string m_adb_path;
    std::string m_address;
    std::string m_uuid;
    bool m_connected = false;
    int m_width = 1280;
    int m_height = 720;

    std::vector<uint8_t> m_screencap_data;
    mutable std::mutex m_mutex;
};

} // namespace asst::yys
