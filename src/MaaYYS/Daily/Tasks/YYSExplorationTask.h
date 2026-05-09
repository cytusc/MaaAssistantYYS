#pragma once
#include "../YYSDailyTask.h"
#include <memory>

#include "Exploration/ExplorationNavigator.h"
#include "Exploration/ChapterSelector.h"
#include "Exploration/ExplorationRunner.h"
#include "Exploration/ExplorationRewardHandler.h"

namespace asst::yys {

class YYSExplorationTask : public YYSDailyTask {
public:
    YYSExplorationTask();
    virtual ~YYSExplorationTask() = default;

    std::string task_id() const override { return "exploration"; }
    std::string name() const override { return "YYSExplorationTask"; }
    int priority() const override { return m_priority; }

    void set_target_chapter(int chapter) { m_target_chapter = chapter; }
    void set_max_nodes(int max_nodes) { m_max_nodes = max_nodes; }

protected:
    bool _run() override;
    bool on_executed() override;

private:
    int m_target_chapter = 28;
    int m_max_nodes = 20;
    int m_priority = 40;

    std::unique_ptr<ExplorationNavigator> m_navigator;
    std::unique_ptr<ChapterSelector> m_chapter_selector;
    std::unique_ptr<ExplorationRunner> m_runner;
    std::unique_ptr<ExplorationRewardHandler> m_reward_handler;
};

} // namespace asst::yys
