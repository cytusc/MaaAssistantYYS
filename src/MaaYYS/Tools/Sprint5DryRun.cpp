#include <iostream>
#include <memory>
#include <optional>
#include <vector>
#include <filesystem>

#include "Base/YYSContext.h"
#include "Common/Controller/MaaControllerActionExecutor.h"
#include "Common/Controller/RecordingActionExecutor.h"
#include "Common/Vision/ScriptedTemplateResolver.h"
#include "Common/Vision/ScriptedOcrResolver.h"
#include "Common/Resource/YYSResourceLoader.h"
#include "Daily/Tasks/YYSWantedTask.h"
#include "Daily/Tasks/YYSCoinMonsterTask.h"
#include "Daily/Tasks/YYSSignInTask.h"

void verify_resources(const std::string& resource_path)
{
    std::cout << "\n=== Resource Verification ===" << std::endl;
    std::cout << "Loading resources from: " << resource_path << std::endl;

    asst::yys::YYSResourceLoader loader;
    bool loaded = loader.load(resource_path);

    if (!loaded) {
        std::cout << "❌ Failed to load resources" << std::endl;
        return;
    }

    std::cout << "✅ Resources loaded successfully" << std::endl;
    std::cout << "Total templates: " << loader.templates().size() << std::endl;
    std::cout << "Total OCR: " << loader.ocrs().size() << std::endl;

    auto check_templates = [&](const std::string& section, const std::vector<std::string>& ids) {
        std::cout << "\n=== " << section << " Templates ===" << std::endl;
        for (const auto& id : ids) {
            auto it = loader.templates().find(id);
            if (it != loader.templates().end()) {
                bool exists = std::filesystem::exists(it->second.file);
                std::cout << (exists ? "✅" : "❌") << " " << id
                          << (exists ? "" : " (file missing)") << std::endl;
            } else {
                std::cout << "❌ " << id << " (not in config)" << std::endl;
            }
        }
    };

    check_templates("Wanted", {
        "I_WANTED", "I_WANTED_TASK", "I_WANTED_ACCEPT",
        "I_WANTED_COMPLETE", "I_WANTED_REWARD"
    });

    check_templates("Coin Monster", {
        "I_COIN_MONSTER", "I_COIN_BATTLE", "I_COIN_REWARD"
    });

    check_templates("Sign In", {
        "I_SIGN_IN", "I_SIGN_IN_REWARD", "I_SIGN_IN_CLOSE"
    });

    // Verify OCR resources
    std::cout << "\n=== OCR Resources ===" << std::endl;
    for (const auto& ocr_id : {"O_WANTED_TASK"}) {
        auto it = loader.ocrs().find(ocr_id);
        std::cout << (it != loader.ocrs().end() ? "✅" : "❌") << " " << ocr_id << std::endl;
    }
}

struct TestContext {
    std::shared_ptr<asst::yys::YYSContext> ctx;
    std::shared_ptr<asst::yys::ScriptedTemplateResolver> resolver;
    std::shared_ptr<asst::yys::ScriptedOcrResolver> ocr_resolver;
    std::shared_ptr<asst::yys::RecordingActionExecutor> recording;
};

TestContext create_test_context()
{
    using namespace asst::yys;
    TestContext tc;
    tc.ctx = std::make_shared<YYSContext>();
    tc.resolver = std::make_shared<ScriptedTemplateResolver>();
    tc.ocr_resolver = std::make_shared<ScriptedOcrResolver>();
    tc.recording = std::make_shared<RecordingActionExecutor>();
    auto executor = std::make_shared<MaaControllerActionExecutor>();

    executor->bind_screencap([tc]() { return tc.recording->screencap(); });
    executor->bind_click([tc](Point p) { return tc.recording->click(p); });
    executor->bind_swipe([tc](Point f, Point t) { return tc.recording->swipe(f, t); });
    executor->bind_long_click([tc](Point p, int d) { return tc.recording->long_click(p, d); });

    tc.ctx->set_resolver(tc.resolver);
    tc.ctx->set_ocr_resolver(tc.ocr_resolver);
    tc.ctx->set_executor(executor);
    return tc;
}

bool test_wanted()
{
    using namespace asst::yys;
    std::cout << "\n=== Testing Wanted Task ===" << std::endl;

    auto tc = create_test_context();

    // Navigator: enter wanted
    tc.resolver->set_sequence("I_WANTED", { Rect{50, 630, 200, 80} });
    // Recognizer: find task, then no more
    tc.resolver->set_sequence("I_WANTED_TASK", {
        Rect{200, 200, 600, 80},
        std::nullopt,
    });
    tc.resolver->set_sequence("I_WANTED_ACCEPT", { Rect{950, 250, 200, 60} });
    // OCR: recognize monster name
    tc.ocr_resolver->set_sequence("O_WANTED_TASK", {
        std::string("跳跳哥哥 x3"),
    });
    // Battle complete + reward
    tc.resolver->set_sequence("I_WANTED_COMPLETE", { Rect{500, 300, 300, 100} });
    tc.resolver->set_sequence("I_WANTED_REWARD", {
        Rect{500, 400, 200, 80},
        std::nullopt,
    });

    YYSWantedTask task;
    task.init(tc.ctx);
    bool ok = task.run();

    std::cout << "wanted_result=" << (ok ? "ok" : "failed") << std::endl;
    std::cout << "recorded_actions=" << tc.recording->actions().size() << std::endl;
    return ok;
}

bool test_coin_monster()
{
    using namespace asst::yys;
    std::cout << "\n=== Testing Coin Monster Task ===" << std::endl;

    auto tc = create_test_context();

    tc.resolver->set_sequence("I_COIN_MONSTER", { Rect{400, 300, 100, 100} });
    tc.resolver->set_sequence("I_COIN_BATTLE", { Rect{900, 550, 200, 100} });
    tc.resolver->set_sequence("I_COIN_REWARD", {
        Rect{500, 400, 200, 80},
        Rect{500, 400, 200, 80},
        std::nullopt,
    });

    YYSCoinMonsterTask task;
    task.init(tc.ctx);
    bool ok = task.run();

    std::cout << "coin_monster_result=" << (ok ? "ok" : "failed") << std::endl;
    std::cout << "recorded_actions=" << tc.recording->actions().size() << std::endl;
    return ok;
}

bool test_sign_in()
{
    using namespace asst::yys;
    std::cout << "\n=== Testing Sign-In Task ===" << std::endl;

    auto tc = create_test_context();

    tc.resolver->set_sequence("I_SIGN_IN", { Rect{600, 400, 150, 60} });
    tc.resolver->set_sequence("I_SIGN_IN_REWARD", {
        Rect{500, 350, 200, 80},
        std::nullopt,
    });
    tc.resolver->set_sequence("I_SIGN_IN_CLOSE", { Rect{1000, 80, 100, 50} });

    YYSSignInTask task;
    task.init(tc.ctx);
    bool ok = task.run();

    std::cout << "sign_in_result=" << (ok ? "ok" : "failed") << std::endl;
    std::cout << "recorded_actions=" << tc.recording->actions().size() << std::endl;
    return ok;
}

int main()
{
    const std::string resource_path = "D:/code/maa/MaaAssistantYYS/resource/YYS";
    verify_resources(resource_path);

    bool all_ok = true;
    all_ok &= test_wanted();
    all_ok &= test_coin_monster();
    all_ok &= test_sign_in();

    std::cout << "\n=== Sprint 5 Summary ===" << std::endl;
    std::cout << "dry_run_result=" << (all_ok ? "ok" : "failed") << std::endl;

    return all_ok ? 0 : 1;
}
