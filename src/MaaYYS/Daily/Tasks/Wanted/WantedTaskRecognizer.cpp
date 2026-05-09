#include "WantedTaskRecognizer.h"

#include <thread>
#include <chrono>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/IOcrResolver.h"
#include "Base/YYSTypes.h"
#include "Common/Logger/YYSLogger.h"
#include "Common/Flow/YYSWaitFlow.h"
#include "Common/Flow/YYSActionFlow.h"
#include "Common/Flow/YYSRetryFlow.h"

namespace asst::yys {

WantedTaskRecognizer::WantedTaskRecognizer(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool WantedTaskRecognizer::find_and_accept_task()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Searching for wanted task...");

    YYSWaitFlow wait_task("I_WANTED_TASK", DEFAULT_TIMEOUT_MS);
    wait_task.init(m_ctx);
    if (!wait_task.run()) {
        YYS_LOG_INFO("No wanted task found");
        return false;
    }

    // OCR to identify the task target
    if (m_ctx->ocr_resolver()) {
        m_ctx->executor()->screencap();
        auto text = m_ctx->ocr_resolver()->recognize("O_WANTED_TASK");
        if (text.has_value()) {
            m_recognized_text = text.value();
            YYS_LOG_INFO("OCR recognized task: %s", m_recognized_text.c_str());
        } else {
            YYS_LOG_WARN("OCR failed, proceeding without text");
        }
    }

    YYSRetryFlow click_accept(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_WANTED_ACCEPT"),
        3);
    click_accept.init(m_ctx);
    if (!click_accept.run()) {
        YYS_LOG_ERROR("Failed to accept wanted task");
        return false;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    YYS_LOG_INFO("Wanted task accepted");
    return true;
}

} // namespace asst::yys
