#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSCoinMonsterTask : public YYSDailyTask {
public:
    YYSCoinMonsterTask();
    virtual ~YYSCoinMonsterTask() = default;

    std::string task_id() const override { return "coin_monster"; }
    std::string name() const override { return "YYSCoinMonsterTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 70;
    static constexpr int DETECT_TIMEOUT_MS = 5000;
    static constexpr int BATTLE_TIMEOUT_MS = 60000;
};

} // namespace asst::yys
