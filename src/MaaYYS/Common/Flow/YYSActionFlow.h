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
    YYSActionFlow(ActionType type, int x, int y, int x2, int y2,
                 const std::string& target = "");
    bool run() override;

private:
    ActionType m_type;
    int m_x;
    int m_y;
    int m_x2;
    int m_y2;
    std::string m_target;
};

} // namespace asst::yys
