#pragma once
#include "../YYSDailyTask.h"
#include <vector>
#include <string>
#include <chrono>
#include <memory>

// 包含组件头文件
#include "Orochi/OrochiNavigator.h"
#include "Orochi/OrochiLayerSelector.h"
#include "Orochi/OrochiBattleRunner.h"
#include "Orochi/RewardHandler.h"

namespace asst::yys {

// 御魂层数枚举
enum class OrochiLayer {
    One = 1, Two = 2, Three = 3, Four = 4, Five = 5,
    Six = 6, Seven = 7, Eight = 8, Nine = 9, Ten = 10
};

class YYSOrochiTask : public YYSDailyTask {
public:
    YYSOrochiTask();
    virtual ~YYSOrochiTask() = default;

    std::string task_id() const override { return "orochi"; }
    std::string name() const override { return "YYSOrochiTask"; }
    int priority() const override { return m_priority; }

    // 配置方法
    void set_target_layer(OrochiLayer layer) { m_target_layer = layer; }
    void set_sweep_enabled(bool enabled) { m_sweep_enabled = enabled; }
    void set_sweep_count(int count) { m_sweep_count = count; }

protected:
    bool _run() override;
    bool on_executed() override;

private:
    // 核心步骤
    bool check_medicine();            // 检查体力

    // 配置
    OrochiLayer m_target_layer = OrochiLayer::Eight;  // 默认第8层
    int m_sweep_count = 0;            // 扫荡次数
    bool m_sweep_enabled = false;     // 是否启用扫荡
    int m_priority = 50;              // 任务优先级

    // 内部状态
    int m_battle_count = 0;           // 已完成战斗次数

    // 拆分的组件
    std::unique_ptr<OrochiNavigator> m_navigator;
    std::unique_ptr<OrochiLayerSelector> m_layer_selector;
    std::unique_ptr<OrochiBattleRunner> m_battle_runner;
    std::unique_ptr<RewardHandler> m_reward_handler;
};

} // namespace asst::yys
