#include <iostream>
#include <memory>
#include <optional>
#include <vector>
#include <filesystem>

#include "Base/YYSContext.h"
#include "Common/Controller/MaaControllerActionExecutor.h"
#include "Common/Controller/RecordingActionExecutor.h"
#include "Common/Vision/ScriptedTemplateResolver.h"
#include "Common/Resource/YYSResourceLoader.h"
#include "Daily/Tasks/YYSOrochiTask.h"

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

    // 验证御魂特定模板
    const std::vector<std::string> orochi_templates = {
        "I_OROCHI",
        "I_OROCHI_FIRE",
        "I_REWARD",
        "O_LAYER_8"
    };

    std::cout << "\n=== Orochi Templates ===" << std::endl;
    for (const auto& template_id : orochi_templates) {
        auto it = loader.templates().find(template_id);
        if (it != loader.templates().end()) {
            const auto& res = it->second;
            bool file_exists = std::filesystem::exists(res.file);
            std::cout << (file_exists ? "✅" : "❌") << " " << template_id
                      << (file_exists ? "" : " (file missing)") << std::endl;
        } else {
            std::cout << "❌ " << template_id << " (not found in config)" << std::endl;
        }
    }
}

int main()
{
    using namespace asst::yys;

    // 验证资源
    const std::string resource_path = "D:/code/maa/MaaAssistantYYS/resource/YYS";
    verify_resources(resource_path);

    // 运行御魂任务测试
    std::cout << "\n=== Running Orochi Task Test ===" << std::endl;

    auto ctx = std::make_shared<YYSContext>();
    auto resolver = std::make_shared<ScriptedTemplateResolver>();
    auto recording = std::make_shared<RecordingActionExecutor>();
    auto executor = std::make_shared<MaaControllerActionExecutor>();

    executor->bind_screencap([recording]() { return recording->screencap(); });
    executor->bind_click([recording](Point point) { return recording->click(point); });
    executor->bind_swipe([recording](Point from, Point to) { return recording->swipe(from, to); });
    executor->bind_long_click([recording](Point point, int duration_ms) { return recording->long_click(point, duration_ms); });

    resolver->set_sequence("I_OROCHI", { Rect { 55, 104, 295, 406 } });
    resolver->set_sequence("O_LAYER_8", { Rect { 138, 130, 45, 88 } });
    resolver->set_sequence("I_OROCHI_FIRE", { Rect { 1095, 577, 131, 124 } });
    resolver->set_sequence("I_REWARD", {
        Rect { 500, 500, 120, 80 },
        Rect { 500, 500, 120, 80 },
        Rect { 500, 500, 120, 80 },
        std::nullopt,
    });

    ctx->set_resolver(resolver);
    ctx->set_executor(executor);

    YYSOrochiTask task;
    task.init(ctx);

    const bool ok = task.run();
    std::cout << "\n=== Test Results ===" << std::endl;
    std::cout << "dry_run_result=" << (ok ? "ok" : "failed") << '\n';
    std::cout << "recorded_actions=" << recording->actions().size() << '\n';

    return ok ? 0 : 1;
}
