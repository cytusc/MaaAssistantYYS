#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSGuildTask : public YYSDailyTask {
public:
    YYSGuildTask();
    virtual ~YYSGuildTask() = default;

    std::string task_id() const override { return "guild"; }
    std::string name() const override { return "YYSGuildTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 25;
    static constexpr int DETECT_TIMEOUT_MS = 5000;
    static constexpr int BATTLE_TIMEOUT_MS = 60000;
};

} // namespace asst::yys
