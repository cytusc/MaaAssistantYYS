#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <functional>

// Minimal test framework (no external dependency)
namespace yys_test {

struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

static std::vector<TestResult> g_results;
static int g_pass = 0;
static int g_fail = 0;

inline void check(bool condition, const std::string& test_name, const std::string& msg = "")
{
    if (condition) {
        ++g_pass;
        g_results.push_back({test_name, true, ""});
    } else {
        ++g_fail;
        g_results.push_back({test_name, false, msg});
        std::cout << "  FAIL: " << test_name << (msg.empty() ? "" : " - " + msg) << std::endl;
    }
}

inline void report()
{
    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Passed: " << g_pass << std::endl;
    std::cout << "Failed: " << g_fail << std::endl;
    std::cout << "Total:  " << (g_pass + g_fail) << std::endl;
    std::cout << "test_result=" << (g_fail == 0 ? "ok" : "failed") << std::endl;
}

} // namespace yys_test

#include "Base/YYSContext.h"
#include "Base/YYSTypes.h"
#include "Base/IActionExecutor.h"
#include "Base/ITemplateResolver.h"
#include "Base/IOcrResolver.h"
#include "Common/Controller/RecordingActionExecutor.h"
#include "Common/Controller/MaaControllerActionExecutor.h"
#include "Common/Vision/StaticTemplateResolver.h"
#include "Common/Vision/ScriptedTemplateResolver.h"
#include "Common/Vision/ScriptedOcrResolver.h"
#include "Common/Flow/YYSWaitFlow.h"
#include "Common/Flow/YYSActionFlow.h"
#include "Common/Flow/YYSRetryFlow.h"
#include "Common/Flow/YYSLoopFlow.h"
#include "Common/Resource/YYSResourceLoader.h"

using namespace asst::yys;
using namespace yys_test;

// ============ StaticTemplateResolver Tests ============
void test_static_resolver()
{
    std::cout << "\n--- StaticTemplateResolver ---" << std::endl;
    StaticTemplateResolver resolver;
    resolver.set_template("A", Rect{10, 20, 30, 40});

    auto r = resolver.find_template("A");
    check(r.has_value(), "static_find_existing");
    check(r->x == 10 && r->y == 20, "static_correct_value");

    auto none = resolver.find_template("B");
    check(!none.has_value(), "static_find_missing");
}

// ============ ScriptedTemplateResolver Tests ============
void test_scripted_resolver()
{
    std::cout << "\n--- ScriptedTemplateResolver ---" << std::endl;
    ScriptedTemplateResolver resolver;
    resolver.set_sequence("X", {
        Rect{1, 2, 3, 4},
        std::nullopt,
        Rect{5, 6, 7, 8},
    });

    auto r1 = resolver.find_template("X");
    check(r1.has_value() && r1->x == 1, "scripted_first");

    auto r2 = resolver.find_template("X");
    check(!r2.has_value(), "scripted_second_nullopt");

    auto r3 = resolver.find_template("X");
    check(r3.has_value() && r3->x == 5, "scripted_third");

    // stays at last
    auto r4 = resolver.find_template("X");
    check(r4.has_value() && r4->x == 5, "scripted_stays_at_last");

    auto none = resolver.find_template("Y");
    check(!none.has_value(), "scripted_unknown_key");
}

// ============ ScriptedOcrResolver Tests ============
void test_scripted_ocr()
{
    std::cout << "\n--- ScriptedOcrResolver ---" << std::endl;
    ScriptedOcrResolver resolver;
    resolver.set_sequence("OCR1", {
        std::string("hello"),
        std::nullopt,
        std::string("world"),
    });

    auto r1 = resolver.recognize("OCR1");
    check(r1.has_value() && r1.value() == "hello", "ocr_first");

    auto r2 = resolver.recognize("OCR1");
    check(!r2.has_value(), "ocr_second_nullopt");

    auto r3 = resolver.recognize("OCR1");
    check(r3.has_value() && r3.value() == "world", "ocr_third");

    auto none = resolver.recognize("UNKNOWN");
    check(!none.has_value(), "ocr_unknown_key");
}

// ============ RecordingActionExecutor Tests ============
void test_recording_executor()
{
    std::cout << "\n--- RecordingActionExecutor ---" << std::endl;
    RecordingActionExecutor exec;

    exec.screencap();
    exec.click(Point{10, 20});
    exec.swipe(Point{0, 0}, Point{100, 200});

    check(exec.actions().size() == 3, "recording_count");
}

// ============ YYSWaitFlow Tests ============
void test_wait_flow()
{
    std::cout << "\n--- YYSWaitFlow ---" << std::endl;

    auto ctx = std::make_shared<YYSContext>();
    auto resolver = std::make_shared<ScriptedTemplateResolver>();
    auto recording = std::make_shared<RecordingActionExecutor>();
    auto executor = std::make_shared<MaaControllerActionExecutor>();
    executor->bind_screencap([recording]() { return recording->screencap(); });
    executor->bind_click([recording](Point p) { return recording->click(p); });
    ctx->set_resolver(resolver);
    ctx->set_executor(executor);

    resolver->set_sequence("T", { Rect{10, 10, 50, 50} });
    YYSWaitFlow wait("T", 2000);
    wait.init(ctx);
    check(wait.run(), "waitflow_success");
    check(wait.found_rect().has_value(), "waitflow_has_rect");

    // timeout case
    resolver->set_sequence("NEVER", { std::nullopt });
    YYSWaitFlow wait2("NEVER", 500);
    wait2.init(ctx);
    check(!wait2.run(), "waitflow_timeout");
}

// ============ YYSRetryFlow Tests ============
void test_retry_flow()
{
    std::cout << "\n--- YYSRetryFlow ---" << std::endl;

    auto ctx = std::make_shared<YYSContext>();
    auto resolver = std::make_shared<ScriptedTemplateResolver>();
    auto recording = std::make_shared<RecordingActionExecutor>();
    auto executor = std::make_shared<MaaControllerActionExecutor>();
    executor->bind_screencap([recording]() { return recording->screencap(); });
    executor->bind_click([recording](Point p) { return recording->click(p); });
    ctx->set_resolver(resolver);
    ctx->set_executor(executor);

    resolver->set_sequence("BTN", { Rect{50, 50, 100, 40} });
    YYSRetryFlow retry(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "BTN"),
        3);
    retry.init(ctx);
    check(retry.run(), "retryflow_success");
}

// ============ YYSResourceLoader Tests ============
void test_resource_loader()
{
    std::cout << "\n--- YYSResourceLoader ---" << std::endl;
    YYSResourceLoader loader;
    bool loaded = loader.load("D:/code/maa/MaaAssistantYYS/resource/YYS");
    check(loaded, "loader_load");
    check(loader.templates().size() > 0, "loader_has_templates");
    check(loader.ocrs().size() > 0, "loader_has_ocrs");
    check(loader.templates().contains("I_OROCHI"), "loader_has_orochi");
    check(loader.ocrs().contains("O_LAYER"), "loader_has_ocr_layer");
}

// ============ YYSLoopFlow Tests ============
void test_loop_flow()
{
    std::cout << "\n--- YYSLoopFlow ---" << std::endl;

    auto ctx = std::make_shared<YYSContext>();
    auto resolver = std::make_shared<ScriptedTemplateResolver>();
    auto recording = std::make_shared<RecordingActionExecutor>();
    auto executor = std::make_shared<MaaControllerActionExecutor>();
    executor->bind_screencap([recording]() { return recording->screencap(); });
    executor->bind_click([recording](Point p) { return recording->click(p); });
    ctx->set_resolver(resolver);
    ctx->set_executor(executor);

    resolver->set_sequence("LOOP_T", {
        Rect{10, 10, 50, 50},
        Rect{10, 10, 50, 50},
        std::nullopt,
    });

    YYSLoopFlow loop(5);
    auto action = std::make_unique<YYSWaitFlow>("LOOP_T", 500);
    loop.set_next(std::move(action));
    loop.init(ctx);
    loop.run();
    check(true, "loopflow_runs_without_crash");
}

int main()
{
    std::cout << "=== MaaYYS Unit Tests ===" << std::endl;

    test_static_resolver();
    test_scripted_resolver();
    test_scripted_ocr();
    test_recording_executor();
    test_wait_flow();
    test_retry_flow();
    test_loop_flow();
    test_resource_loader();

    report();
    return g_fail == 0 ? 0 : 1;
}
