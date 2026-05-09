#pragma once

#include <string>
#include <functional>
#include <memory>

#include "Base/IActionExecutor.h"
#include "Base/ITemplateResolver.h"
#include "Base/YYSTypes.h"

namespace asst::yys {

struct MaaCoreConnectionParams {
    std::string adb_path;
    std::string address;
    std::string config = "General";
    std::string touch_mode = "minitouch";
    bool adb_lite = false;
};

class MaaCoreBridge {
public:
    using ScreencapCallback = std::function<bool(const void* data, int width, int height, int channels)>;
    using LogCallback = std::function<void(const std::string& level, const std::string& message)>;

    MaaCoreBridge() = default;
    virtual ~MaaCoreBridge() = default;

    virtual bool connect(const MaaCoreConnectionParams& params) = 0;
    virtual bool connected() const = 0;
    virtual void disconnect() = 0;

    virtual bool screencap() = 0;
    virtual bool click(int x, int y) = 0;
    virtual bool swipe(int x1, int y1, int x2, int y2, int duration = 300) = 0;
    virtual bool long_click(int x, int y, int duration = 800) = 0;
    virtual bool press_key(int key) = 0;
    virtual bool input_text(const std::string& text) = 0;

    virtual bool start_game(const std::string& client_type) = 0;
    virtual bool stop_game(const std::string& client_type) = 0;

    virtual int screen_width() const = 0;
    virtual int screen_height() const = 0;
    virtual std::string uuid() const = 0;

    void set_screencap_callback(ScreencapCallback cb) { m_screencap_cb = std::move(cb); }
    void set_log_callback(LogCallback cb) { m_log_cb = std::move(cb); }

protected:
    ScreencapCallback m_screencap_cb;
    LogCallback m_log_cb;
};

class MaaCoreBridgeFactory {
public:
    static std::unique_ptr<MaaCoreBridge> create();
};

} // namespace asst::yys
