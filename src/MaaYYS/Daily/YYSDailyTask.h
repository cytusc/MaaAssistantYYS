#pragma once

#include "Base/IYYSDaily.h"
#include "Base/YYSModule.h"

namespace asst::yys {

class YYSDailyTask : public YYSModule, public IYYSDaily {
public:
    bool run() override;
    int priority() const override { return m_priority; }
    bool check_enable() const override;

protected:
    virtual bool _run() = 0;
    virtual bool check_resource() const;
    virtual bool on_executed();

    int m_priority = 0;
};

} // namespace asst::yys
