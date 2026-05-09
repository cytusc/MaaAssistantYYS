#pragma once
#include "YYSFlowBase.h"

namespace asst::yys {

class YYSWaitFlow : public YYSFlowBase {
public:
    YYSWaitFlow(const std::string& target, int timeout_ms = 30000);
    bool run() override;
    
private:
    std::string m_target;
    int m_timeout_ms;
};

} // namespace asst::yys
