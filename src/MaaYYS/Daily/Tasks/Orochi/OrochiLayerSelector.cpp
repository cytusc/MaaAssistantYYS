#include "OrochiLayerSelector.h"

#include <thread>
#include <chrono>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSTypes.h"
#include "Common/Logger/YYSLogger.h"
#include "Common/Flow/YYSWaitFlow.h"
#include "Common/Flow/YYSActionFlow.h"
#include "Common/Flow/YYSRetryFlow.h"

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

    if (find_layer_directly(layer_num)) {
        YYS_LOG_INFO("Layer %d found directly", layer_num);
        const std::string layer_target = "O_LAYER_" + std::to_string(layer_num);
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
    const std::string layer_target = "O_LAYER_" + std::to_string(layer_num);
    YYSWaitFlow wait_flow(layer_target, DEFAULT_TIMEOUT_MS);
    wait_flow.init(m_ctx);
    return wait_flow.run();
}

bool OrochiLayerSelector::swipe_to_find_layer(int target_layer)
{
    if (!m_ctx || !m_ctx->executor()) {
        YYS_LOG_ERROR("Executor not available for swipe");
        return false;
    }

    YYS_LOG_INFO("Swiping to find layer: %d", target_layer);

    const bool swipe_up = target_layer <= 5;

    for (int i = 0; i < 3; ++i) {
        if (find_layer_directly(target_layer)) {
            YYS_LOG_INFO("Found layer %d after %d swipes", target_layer, i);
            const std::string layer_target = "O_LAYER_" + std::to_string(target_layer);
            return click_layer(layer_target);
        }

        YYSActionFlow swipe_flow(
            ActionType::Swipe,
            640, swipe_up ? 520 : 240,
            640, swipe_up ? 240 : 520);
        swipe_flow.init(m_ctx);
        swipe_flow.run();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    YYS_LOG_WARN("Layer %d not found after swiping", target_layer);
    return false;
}

bool OrochiLayerSelector::click_layer(const std::string& layer_target)
{
    YYSRetryFlow click_flow(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, layer_target),
        3);
    click_flow.init(m_ctx);
    if (!click_flow.run()) {
        YYS_LOG_ERROR("Failed to click %s", layer_target.c_str());
        return false;
    }
    YYS_LOG_INFO("Clicked %s", layer_target.c_str());
    return true;
}

} // namespace asst::yys
