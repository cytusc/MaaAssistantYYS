#pragma once
#include <string>

namespace asst::yys {

class IYYSDaily {
public:
    virtual ~IYYSDaily() = default;
    virtual std::string task_id() const = 0;
    virtual int priority() const { return 0; }
    virtual bool run() = 0;
    virtual bool check_enable() const { return true; }
};

} // namespace asst::yys
