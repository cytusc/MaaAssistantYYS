#pragma once
#include <memory>
#include <vector>
#include "Base/IYYSDaily.h"

namespace asst::yys {

class YYSDailyManager {
public:
    YYSDailyManager();
    ~YYSDailyManager();
    
    void register_daily(std::unique_ptr<IYYSDaily> daily);
    bool run_all();
    bool run_by_priority(int min_priority);
    void register_default_tasks();  // 注册默认任务
    
private:
    std::vector<std::unique_ptr<IYYSDaily>> m_dailies;
};

} // namespace asst::yys
