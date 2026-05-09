#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSTrueSnakeTask : public YYSDailyTask {
public:
    YYSTrueSnakeTask();
    virtual ~YYSTrueSnakeTask() = default;

    std::string task_id() const override { return "true_snake"; }
    std::string name() const override { return "YYSTrueSnakeTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 45;
    static constexpr int DETECT_TIMEOUT_MS = 5000;
    static constexpr int BATTLE_TIMEOUT_MS = 120000;
};

} // namespace asst::yys
