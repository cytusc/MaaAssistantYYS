#pragma once
#include "../YYSDailyTask.h"
#include <memory>

#include "Wanted/WantedNavigator.h"
#include "Wanted/WantedTaskRecognizer.h"
#include "Wanted/WantedRewardHandler.h"

namespace asst::yys {

class YYSWantedTask : public YYSDailyTask {
public:
    YYSWantedTask();
    virtual ~YYSWantedTask() = default;

    std::string task_id() const override { return "wanted"; }
    std::string name() const override { return "YYSWantedTask"; }
    int priority() const override { return m_priority; }

    void set_max_tasks(int max_tasks) { m_max_tasks = max_tasks; }

protected:
    bool _run() override;
    bool on_executed() override;

private:
    int m_priority = 60;
    int m_max_tasks = 5;
    int m_tasks_completed = 0;

    std::unique_ptr<WantedNavigator> m_navigator;
    std::unique_ptr<WantedTaskRecognizer> m_recognizer;
    std::unique_ptr<WantedRewardHandler> m_reward_handler;
};

} // namespace asst::yys
