#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSSecretDungeonTask : public YYSDailyTask {
public:
    YYSSecretDungeonTask();
    virtual ~YYSSecretDungeonTask() = default;

    std::string task_id() const override { return "secret_dungeon"; }
    std::string name() const override { return "YYSSecretDungeonTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 35;
    static constexpr int DETECT_TIMEOUT_MS = 5000;
    static constexpr int BATTLE_TIMEOUT_MS = 120000;
};

} // namespace asst::yys
