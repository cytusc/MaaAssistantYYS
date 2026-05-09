#include "YYSOrochiTask.h"
#include "Orochi/OrochiNavigator.h"
#include "Orochi/OrochiLayerSelector.h"
#include "Orochi/OrochiBattleRunner.h"
#include "Orochi/RewardHandler.h"

#include <chrono>
#include <cstdio>
#include <thread>

#include "Base/IActionExecutor.h"
#include "Base/ITemplateResolver.h"
#include "Base/YYSContext.h"

#ifdef _WIN32
#define LOG_INFO(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) printf("[WARN] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) printf("[WARN] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#endif

namespace asst::yys {

YYSOrochiTask::YYSOrochiTask()
    : m_priority(50)
    , m_battle_count(0)
{
    LOG_INFO("YYSOrochiTask constructed, target layer: %d", static_cast<int>(m_target_layer));
}

bool YYSOrochiTask::_run()
{
    LOG_INFO("========== YYSOrochiTask started ==========");
    LOG_INFO("Target layer: %d, Sweep enabled: %s, Sweep count: %d",
             static_cast<int>(m_target_layer),
             m_sweep_enabled ? "true" : "false",
             m_sweep_count);

    if (!check_enable()) {
        LOG_ERROR("Enable check failed");
        return false;
    }

    if (!check_resource()) {
        LOG_ERROR("Resource check failed");
        return false;
    }

    if (!check_medicine()) {
        LOG_ERROR("Medicine check failed, not enough stamina");
        return false;
    }

    // 初始化组件
    m_navigator = std::make_unique<OrochiNavigator>(m_ctx);
    m_layer_selector = std::make_unique<OrochiLayerSelector>(m_ctx);
    m_battle_runner = std::make_unique<OrochiBattleRunner>(m_ctx);
    m_reward_handler = std::make_unique<RewardHandler>(m_ctx);

    if (!m_navigator->enter()) {
        LOG_ERROR("Failed to enter orochi interface");
        return false;
    }

    if (!m_layer_selector->select(static_cast<int>(m_target_layer))) {
        LOG_ERROR("Failed to select layer: %d", static_cast<int>(m_target_layer));
        return false;
    }

    const int max_battles = m_sweep_enabled ? m_sweep_count : 1;

    for (int i = 0; i < max_battles; ++i) {
        LOG_INFO("Starting battle #%d", i + 1);

        if (!m_battle_runner->start()) {
            LOG_ERROR("Failed to start battle #%d", i + 1);
            return false;
        }

        if (!m_battle_runner->wait_for_end()) {
            LOG_ERROR("Failed to wait battle #%d end", i + 1);
            return false;
        }

        ++m_battle_count;
        LOG_INFO("Battle #%d completed", i + 1);
        m_reward_handler->handle();

        if (i < max_battles - 1) {
            if (!check_medicine()) {
                LOG_WARN("Not enough stamina for next battle, stopping");
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    LOG_INFO("========== YYSOrochiTask completed, total battles: %d ==========", m_battle_count);
    return true;
}

bool YYSOrochiTask::on_executed()
{
    LOG_INFO("YYSOrochiTask on_executed called, battles completed: %d", m_battle_count);
    return true;
}

bool YYSOrochiTask::check_medicine()
{
    LOG_DEBUG("Checking medicine/stamina...");
    LOG_DEBUG("Medicine check passed (placeholder)");
    return true;
}

} // namespace asst::yys
