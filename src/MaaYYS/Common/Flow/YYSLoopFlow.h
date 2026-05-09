#pragma once
#include "YYSFlowBase.h"

namespace asst::yys {

class YYSLoopFlow : public YYSFlowBase {
public:
    YYSLoopFlow(int max_loop = -1);  // -1 表示无限循环
    bool run() override;
    
private:
    int m_max_loop;
};

} // namespace asst::yys
