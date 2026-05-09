#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSNianBeastTask : public YYSDailyTask {
public:
    YYSNianBeastTask();
    virtual ~YYSNianBeastTask() = default;

    std::string task_id() const override { return "nian_beast"; }
    std::string name() const override { return "YYSNianBeastTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 35;
    static constexpr int DETECT_TIMEOUT_MS = 5000;
    static constexpr int BATTLE_TIMEOUT_MS = 60000;
};

} // namespace asst::yys
