#include "OrochiBattleRunner.h"

#include <thread>
#include <chrono>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSTypes.h"
#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

OrochiBattleRunner::OrochiBattleRunner(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool OrochiBattleRunner::start()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Starting battle...");

    if (!click_start_button()) {
        YYS_LOG_ERROR("Failed to start battle");
        return false;
    }

    YYS_LOG_INFO("Battle started successfully");
    return true;
}

bool OrochiBattleRunner::wait_for_end()
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Waiting for battle to end...");

    if (!wait_for_reward_screen()) {
        YYS_LOG_ERROR("Battle reward screen not found (timeout)");
        return false;
    }

    YYS_LOG_INFO("Battle ended, reward screen detected");
    return true;
}

bool OrochiBattleRunner::click_start_button()
{
    if (!m_ctx || !m_ctx->resolver() || !m_ctx->executor()) {
        YYS_LOG_ERROR("Context, resolver or executor not available");
        return false;
    }

    const char* target = "I_OROCHI_FIRE";
    constexpr int max_retry = 3;

    for (int i = 0; i < max_retry; ++i) {
        YYS_LOG_DEBUG("Click attempt %d for target: %s", i + 1, target);

        const auto rect = m_ctx->resolver()->find_template(target);
        if (!rect || rect->empty()) {
            YYS_LOG_DEBUG("Template not found: %s", target);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            continue;
        }

        const int x = rect->x + rect->width / 2;
        const int y = rect->y + rect->height / 2;
        YYS_LOG_DEBUG("Target %s found at (%d, %d), size: %dx%d", target, x, y, rect->width, rect->height);

        if (m_ctx->executor()->click(Point { x, y })) {
            YYS_LOG_INFO("Clicked %s at (%d, %d)", target, x, y);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return true;
        }

        YYS_LOG_WARN("Click failed for %s, attempt: %d", target, i + 1);
        m_ctx->executor()->screencap();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    YYS_LOG_ERROR("Failed to click %s after %d attempts", target, max_retry);
    return false;
}

bool OrochiBattleRunner::wait_for_reward_screen()
{
    if (!m_ctx || !m_ctx->resolver() || !m_ctx->executor()) {
        YYS_LOG_ERROR("Context, resolver or executor not available");
        return false;
    }

    const auto start = std::chrono::steady_clock::now();
    constexpr int poll_interval = 500;
    const char* target = "I_REWARD";

    YYS_LOG_DEBUG("Waiting for target: %s, timeout: %dms", target, BATTLE_TIMEOUT_MS);

    while (true) {
        const auto rect = m_ctx->resolver()->find_template(target);
        if (rect && !rect->empty()) {
            YYS_LOG_DEBUG("Target %s found at (%d, %d)", target, rect->x, rect->y);
            return true;
        }

        const auto elapsed = std::chrono::steady_clock::now() - start;
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if (elapsed_ms >= BATTLE_TIMEOUT_MS) {
            YYS_LOG_ERROR("Wait timeout for target: %s", target);
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval));
        m_ctx->executor()->screencap();
    }
}

} // namespace asst::yys
