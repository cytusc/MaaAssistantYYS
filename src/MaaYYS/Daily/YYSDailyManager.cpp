#include "YYSDailyManager.h"
#include "Tasks/YYSOrochiTask.h"
#include "Tasks/YYSExplorationTask.h"
#include "Tasks/YYSWantedTask.h"
#include "Tasks/YYSCoinMonsterTask.h"
#include "Tasks/YYSSignInTask.h"
#include "Tasks/YYSNianBeastTask.h"
#include "Tasks/YYSPetsTask.h"
#include "Tasks/YYSGuildTask.h"
#include "Tasks/YYSAreaBossTask.h"
#include "Tasks/YYSSecretDungeonTask.h"
#include "Tasks/YYSTrueSnakeTask.h"
#include "Tasks/YYSSoulTask.h"
#include "Tasks/YYSAwakeningTask.h"
#include <algorithm>

namespace asst::yys {

YYSDailyManager::YYSDailyManager()
{
    register_default_tasks();
}

YYSDailyManager::~YYSDailyManager()
{
}

void YYSDailyManager::register_daily(std::unique_ptr<IYYSDaily> daily)
{
    if (daily) {
        m_dailies.push_back(std::move(daily));
    }
}

bool YYSDailyManager::run_all()
{
    std::sort(m_dailies.begin(), m_dailies.end(),
        [](const auto& a, const auto& b) {
            return a->priority() > b->priority();
        });

    for (auto& daily : m_dailies) {
        if (daily && daily->check_enable()) {
            daily->run();
        }
    }
    return true;
}

bool YYSDailyManager::run_by_priority(int min_priority)
{
    for (auto& daily : m_dailies) {
        if (daily && daily->check_enable() && daily->priority() >= min_priority) {
            daily->run();
        }
    }
    return true;
}

void YYSDailyManager::register_default_tasks()
{
    register_daily(std::make_unique<YYSSignInTask>());
    register_daily(std::make_unique<YYSCoinMonsterTask>());
    register_daily(std::make_unique<YYSWantedTask>());
    register_daily(std::make_unique<YYSOrochiTask>());
    register_daily(std::make_unique<YYSExplorationTask>());
    register_daily(std::make_unique<YYSAreaBossTask>());
    register_daily(std::make_unique<YYSTrueSnakeTask>());
    register_daily(std::make_unique<YYSSoulTask>());
    register_daily(std::make_unique<YYSAwakeningTask>());
    register_daily(std::make_unique<YYSNianBeastTask>());
    register_daily(std::make_unique<YYSSecretDungeonTask>());
    register_daily(std::make_unique<YYSPetsTask>());
    register_daily(std::make_unique<YYSGuildTask>());
}

} // namespace asst::yys
