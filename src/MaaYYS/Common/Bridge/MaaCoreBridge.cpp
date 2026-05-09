#include "MaaCoreBridge.h"

namespace asst::yys {

class NullMaaCoreBridge : public MaaCoreBridge {
public:
    bool connect(const MaaCoreConnectionParams&) override { return false; }
    bool connected() const override { return false; }
    void disconnect() override {}

    bool screencap() override { return false; }
    bool click(int, int) override { return false; }
    bool swipe(int, int, int, int, int) override { return false; }
    bool long_click(int, int, int) override { return false; }
    bool press_key(int) override { return false; }
    bool input_text(const std::string&) override { return false; }

    bool start_game(const std::string&) override { return false; }
    bool stop_game(const std::string&) override { return false; }

    int screen_width() const override { return 0; }
    int screen_height() const override { return 0; }
    std::string uuid() const override { return ""; }
};

std::unique_ptr<MaaCoreBridge> MaaCoreBridgeFactory::create()
{
    return std::make_unique<NullMaaCoreBridge>();
}

} // namespace asst::yys
