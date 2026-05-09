#include "RewardHandler.h"

#include <thread>
#include <chrono>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSTypes.h"

#ifdef _WIN32
#define LOG_INFO(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#endif

namespace asst::yys {

RewardHandler::RewardHandler(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool RewardHandler::handle()
{
    if (!m_ctx) {
        LOG_ERROR("Context not available");
        return false;
    }

    LOG_INFO("Handling reward...");

    int attempts = 0;
    constexpr int max_attempts = 10;
    while (attempts < max_attempts) {
        if (click_reward_button()) {
            LOG_INFO("Clicked reward button, attempt: %d", attempts + 1);
            ++attempts;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            continue;
        }
        break;
    }

    LOG_INFO("Reward handling completed, attempts: %d", attempts);
    return true;
}

bool RewardHandler::click_reward_button()
{
    if (!m_ctx || !m_ctx->resolver() || !m_ctx->executor()) {
        LOG_ERROR("Context, resolver or executor not available");
        return false;
    }

    const char* target = "I_REWARD";

    // 先等待目标出现
    const auto start = std::chrono::steady_clock::now();
    constexpr int poll_interval = 200;

    while (true) {
        const auto rect = m_ctx->resolver()->find_template(target);
        if (rect && !rect->empty()) {
            break; // 目标出现，准备点击
        }

        const auto elapsed = std::chrono::steady_clock::now() - start;
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if (elapsed_ms >= DEFAULT_TIMEOUT_MS) {
            return false; // 超时，目标未出现
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval));
        m_ctx->executor()->screencap();
    }

    // 点击目标
    constexpr int max_retry = 2;
    for (int i = 0; i < max_retry; ++i) {
        LOG_DEBUG("Click attempt %d for target: %s", i + 1, target);

        const auto rect = m_ctx->resolver()->find_template(target);
        if (!rect || rect->empty()) {
            LOG_DEBUG("Template not found: %s", target);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        const int x = rect->x + rect->width / 2;
        const int y = rect->y + rect->height / 2;

        if (m_ctx->executor()->click(Point { x, y })) {
            LOG_DEBUG("Clicked %s at (%d, %d)", target, x, y);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return true;
        }

        LOG_DEBUG("Click failed for %s, attempt: %d", target, i + 1);
        m_ctx->executor()->screencap();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return false;
}

} // namespace asst::yys
