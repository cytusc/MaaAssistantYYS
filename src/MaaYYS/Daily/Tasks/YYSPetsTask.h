#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSPetsTask : public YYSDailyTask {
public:
    YYSPetsTask();
    virtual ~YYSPetsTask() = default;

    std::string task_id() const override { return "pets"; }
    std::string name() const override { return "YYSPetsTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 30;
    static constexpr int DETECT_TIMEOUT_MS = 5000;
    static constexpr int BATTLE_TIMEOUT_MS = 60000;
};

} // namespace asst::yys
