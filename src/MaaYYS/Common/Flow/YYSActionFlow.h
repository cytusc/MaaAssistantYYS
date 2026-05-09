#pragma once
#include "YYSFlowBase.h"

namespace asst::yys {

enum class ActionType {
    Click,
    Swipe,
    LongClick,
    Text
};

class YYSActionFlow : public YYSFlowBase {
public:
    YYSActionFlow(ActionType type, int x, int y, 
                 const std::string& target = "");
    bool run() override;
    
private:
    ActionType m_type;
    int m_x;
    int m_y;
    std::string m_target;
};

} // namespace asst::yys
