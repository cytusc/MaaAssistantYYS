#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Base/IYYSActivity.h"

namespace asst::yys {

class YYSActivityManager {
public:
    void register_activity(std::unique_ptr<IYYSActivity> activity);
    bool run_all();
    std::vector<std::string> get_tasks() const;

private:
    std::vector<std::unique_ptr<IYYSActivity>> m_activities;
};

} // namespace asst::yys
