#pragma once
#include <string>
#include <vector>

namespace asst::yys {

class IYYSActivity {
public:
    virtual ~IYYSActivity() = default;
    virtual std::string activity_id() const = 0;
    virtual std::string activity_name() const = 0;
    virtual bool run() = 0;
    virtual std::vector<std::string> depends() const { return {}; }
};

} // namespace asst::yys
