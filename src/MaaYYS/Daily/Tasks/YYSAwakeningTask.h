#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSAwakeningTask : public YYSDailyTask {
public:
    YYSAwakeningTask();
    virtual ~YYSAwakeningTask() = default;

    std::string task_id() const override { return "awakening"; }
    std::string name() const override { return "YYSAwakeningTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 40;
    static constexpr int DETECT_TIMEOUT_MS = 5000;
    static constexpr int BATTLE_TIMEOUT_MS = 120000;
};

} // namespace asst::yys
