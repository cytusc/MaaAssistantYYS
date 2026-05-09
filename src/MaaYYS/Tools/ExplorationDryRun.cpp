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
#include "Daily/Tasks/YYSExplorationTask.h"

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

    const std::vector<std::string> exploration_templates = {
        "I_EXPLORATION",
        "I_CHAPTER",
        "I_EXPLORE_FIGHT",
        "I_EXPLORE_MONSTER",
        "I_EXPLORE_BOSS",
        "I_EXPLORE_REWARD",
        "I_EXPLORE_COMPLETE"
    };

    std::cout << "\n=== Exploration Templates ===" << std::endl;
    for (const auto& template_id : exploration_templates) {
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

    const std::string resource_path = "D:/code/maa/MaaAssistantYYS/resource/YYS";
    verify_resources(resource_path);

    std::cout << "\n=== Running Exploration Task Test ===" << std::endl;

    auto ctx = std::make_shared<YYSContext>();
    auto resolver = std::make_shared<ScriptedTemplateResolver>();
    auto recording = std::make_shared<RecordingActionExecutor>();
    auto executor = std::make_shared<MaaControllerActionExecutor>();

    executor->bind_screencap([recording]() { return recording->screencap(); });
    executor->bind_click([recording](Point point) { return recording->click(point); });
    executor->bind_swipe([recording](Point from, Point to) { return recording->swipe(from, to); });
    executor->bind_long_click([recording](Point point, int duration_ms) { return recording->long_click(point, duration_ms); });

    // ExplorationNavigator: wait I_EXPLORATION -> click I_EXPLORATION
    resolver->set_sequence("I_EXPLORATION", { Rect { 50, 200, 200, 150 } });

    // ChapterSelector: wait I_CHAPTER -> click I_CHAPTER
    resolver->set_sequence("I_CHAPTER", { Rect { 400, 300, 300, 80 } });

    // ExplorationRunner::enter_chapter: click I_EXPLORE_FIGHT
    resolver->set_sequence("I_EXPLORE_FIGHT", { Rect { 1000, 550, 200, 100 } });

    // ExplorationRunner::explore_loop:
    //   - complete check fails (nullopt)
    //   - 2 monster nodes found and battled
    //   - 1 boss node found and battled
    //   - then complete detected
    resolver->set_sequence("I_EXPLORE_COMPLETE", {
        std::nullopt,    // iteration 0: not complete
        std::nullopt,    // iteration 1: not complete
        std::nullopt,    // iteration 2: not complete
        Rect { 500, 300, 300, 100 },  // iteration 3: complete
    });
    resolver->set_sequence("I_EXPLORE_BOSS", {
        std::nullopt,    // iteration 0: no boss
        std::nullopt,    // iteration 1: no boss
        Rect { 640, 200, 100, 100 },  // iteration 2: boss found
    });
    resolver->set_sequence("I_EXPLORE_MONSTER", {
        Rect { 300, 250, 80, 80 },    // iteration 0: monster
        Rect { 600, 350, 80, 80 },    // iteration 1: monster
    });

    // wait_battle_end: reward appears after each battle
    resolver->set_sequence("I_EXPLORE_REWARD", {
        Rect { 500, 400, 200, 80 },   // battle 1 reward
        Rect { 500, 400, 200, 80 },   // battle 2 reward
        Rect { 500, 400, 200, 80 },   // battle 3 reward (boss)
        Rect { 500, 400, 200, 80 },   // final reward handler click 1
        Rect { 500, 400, 200, 80 },   // final reward handler click 2
        std::nullopt,                  // reward dismissed
    });

    ctx->set_resolver(resolver);
    ctx->set_executor(executor);

    YYSExplorationTask task;
    task.init(ctx);

    const bool ok = task.run();
    std::cout << "\n=== Test Results ===" << std::endl;
    std::cout << "dry_run_result=" << (ok ? "ok" : "failed") << '\n';
    std::cout << "recorded_actions=" << recording->actions().size() << '\n';

    return ok ? 0 : 1;
}
