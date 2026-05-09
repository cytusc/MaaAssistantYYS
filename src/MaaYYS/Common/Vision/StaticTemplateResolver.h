#pragma once

#include <string>
#include <unordered_map>

#include "Base/ITemplateResolver.h"

namespace asst::yys {

class StaticTemplateResolver : public ITemplateResolver {
public:
    void set_template(std::string name, Rect rect);
    void remove_template(const std::string& name);

    std::optional<Rect> find_template(const std::string& name) const override;

private:
    std::unordered_map<std::string, Rect> m_templates;
};

} // namespace asst::yys
