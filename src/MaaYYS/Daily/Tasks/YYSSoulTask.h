#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSSoulTask : public YYSDailyTask {
public:
    YYSSoulTask();
    virtual ~YYSSoulTask() = default;

    std::string task_id() const override { return "soul"; }
    std::string name() const override { return "YYSSoulTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 40;
    static constexpr int DETECT_TIMEOUT_MS = 5000;
    static constexpr int BATTLE_TIMEOUT_MS = 120000;
};

} // namespace asst::yys
