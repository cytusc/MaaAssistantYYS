#pragma once
#include "YYSFlowBase.h"

namespace asst::yys {

class YYSRetryFlow : public YYSFlowBase {
public:
    YYSRetryFlow(std::unique_ptr<YYSFlowBase> flow, int max_retry = 3);
    bool run() override;
    
private:
    std::unique_ptr<YYSFlowBase> m_flow;
    int m_max_retry;
};

} // namespace asst::yys
