#include "OrochiLayerSelector.h"

#include <thread>
#include <chrono>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSTypes.h"
#include "Common/Logger/YYSLogger.h"

namespace asst::yys {

OrochiLayerSelector::OrochiLayerSelector(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool OrochiLayerSelector::select(int layer_num)
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Selecting layer: %d", layer_num);

    const std::string layer_target = "O_LAYER_" + std::to_string(layer_num);
    if (find_layer_directly(layer_num)) {
        YYS_LOG_INFO("Layer %d found directly", layer_num);
        return click_layer(layer_target);
    }

    YYS_LOG_INFO("Layer not found directly, trying to swipe...");
    if (swipe_to_find_layer(layer_num)) {
        return true;
    }

    YYS_LOG_ERROR("Failed to select layer: %d", layer_num);
    return false;
}

bool OrochiLayerSelector::find_layer_directly(int layer_num)
{
    if (!m_ctx || !m_ctx->resolver() || !m_ctx->executor()) {
        YYS_LOG_ERROR("Context, resolver or executor not available");
        return false;
    }

    const auto start = std::chrono::steady_clock::now();
    constexpr int poll_interval = 500;
    const std::string layer_target = "O_LAYER_" + std::to_string(layer_num);

    YYS_LOG_DEBUG("Waiting for target: %s, timeout: %dms", layer_target.c_str(), DEFAULT_TIMEOUT_MS);

    while (true) {
        const auto rect = m_ctx->resolver()->find_template(layer_target);
        if (rect && !rect->empty()) {
            YYS_LOG_DEBUG("Target %s found at (%d, %d)", layer_target.c_str(), rect->x, rect->y);
            return true;
        }

        const auto elapsed = std::chrono::steady_clock::now() - start;
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if (elapsed_ms >= DEFAULT_TIMEOUT_MS) {
            YYS_LOG_DEBUG("Layer %d not found directly", layer_num);
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval));
        m_ctx->executor()->screencap();
    }
}

bool OrochiLayerSelector::swipe_to_find_layer(int target_layer)
{
    if (!m_ctx || !m_ctx->executor()) {
        YYS_LOG_ERROR("Executor not available for swipe");
        return false;
    }

    YYS_LOG_INFO("Swiping to find layer: %d", target_layer);

    const bool swipe_up = target_layer <= 5;
    const Point from { 640, swipe_up ? 520 : 240 };
    const Point to { 640, swipe_up ? 240 : 520 };

    for (int i = 0; i < 3; ++i) {
        const std::string layer_target = "O_LAYER_" + std::to_string(target_layer);
        if (find_layer_directly(target_layer)) {
            YYS_LOG_INFO("Found layer %d after %d swipes", target_layer, i);
            return click_layer(layer_target);
        }

        YYS_LOG_DEBUG("Performing swipe %d, direction: %s", i, swipe_up ? "up" : "down");
        if (!m_ctx->executor()->swipe(from, to)) {
            YYS_LOG_WARN("Swipe failed on attempt %d", i + 1);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    YYS_LOG_WARN("Layer %d not found after swiping", target_layer);
    return false;
}

bool OrochiLayerSelector::click_layer(const std::string& layer_target)
{
    if (!m_ctx || !m_ctx->resolver() || !m_ctx->executor()) {
        YYS_LOG_ERROR("Context, resolver or executor not available");
        return false;
    }

    constexpr int max_retry = 3;

    for (int i = 0; i < max_retry; ++i) {
        YYS_LOG_DEBUG("Click attempt %d for target: %s", i + 1, layer_target.c_str());

        const auto rect = m_ctx->resolver()->find_template(layer_target);
        if (!rect || rect->empty()) {
            YYS_LOG_DEBUG("Template not found: %s", layer_target.c_str());
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            continue;
        }

        const int x = rect->x + rect->width / 2;
        const int y = rect->y + rect->height / 2;
        YYS_LOG_DEBUG("Target %s found at (%d, %d), size: %dx%d", layer_target.c_str(), x, y, rect->width, rect->height);

        if (m_ctx->executor()->click(Point { x, y })) {
            YYS_LOG_INFO("Clicked %s at (%d, %d)", layer_target.c_str(), x, y);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return true;
        }

        YYS_LOG_WARN("Click failed for %s, attempt: %d", layer_target.c_str(), i + 1);
        m_ctx->executor()->screencap();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    YYS_LOG_ERROR("Failed to click %s after %d attempts", layer_target.c_str(), max_retry);
    return false;
}

} // namespace asst::yys
