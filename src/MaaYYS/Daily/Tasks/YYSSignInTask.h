#pragma once
#include "../YYSDailyTask.h"

namespace asst::yys {

class YYSSignInTask : public YYSDailyTask {
public:
    YYSSignInTask();
    virtual ~YYSSignInTask() = default;

    std::string task_id() const override { return "sign_in"; }
    std::string name() const override { return "YYSSignInTask"; }
    int priority() const override { return m_priority; }

protected:
    bool _run() override;

private:
    int m_priority = 80;
    static constexpr int DEFAULT_TIMEOUT_MS = 10000;
};

} // namespace asst::yys
