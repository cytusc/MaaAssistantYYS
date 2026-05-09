#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSAreaBossTask : public YYSDailyTask {
public:
    YYSAreaBossTask();
    virtual ~YYSAreaBossTask() = default;

    std::string task_id() const override { return "area_boss"; }
    std::string name() const override { return "YYSAreaBossTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 45;
    static constexpr int DETECT_TIMEOUT_MS = 5000;
    static constexpr int BATTLE_TIMEOUT_MS = 120000;
};

} // namespace asst::yys
