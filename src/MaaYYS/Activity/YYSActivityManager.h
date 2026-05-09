#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>
#include <filesystem>

#include "Base/IYYSActivity.h"

namespace asst::yys {

class YYSContext;

class YYSActivityManager {
public:
    void register_activity(std::unique_ptr<IYYSActivity> activity);
    bool run_all();
    std::vector<std::string> get_tasks() const;

    void set_enabled(const std::string& activity_id, bool enabled);
    bool is_enabled(const std::string& activity_id) const;

    bool discover_and_register(const std::filesystem::path& activity_dir,
                               std::shared_ptr<YYSContext> ctx);

private:
    std::vector<std::unique_ptr<IYYSActivity>> m_activities;
    std::unordered_set<std::string> m_disabled;
};

} // namespace asst::yys
