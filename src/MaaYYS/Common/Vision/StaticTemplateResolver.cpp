#include "StaticTemplateResolver.h"

namespace asst::yys {

void StaticTemplateResolver::set_template(std::string name, Rect rect)
{
    m_templates.insert_or_assign(std::move(name), rect);
}

void StaticTemplateResolver::remove_template(const std::string& name)
{
    m_templates.erase(name);
}

std::optional<Rect> StaticTemplateResolver::find_template(const std::string& name) const
{
    const auto iter = m_templates.find(name);
    if (iter == m_templates.end()) {
        return std::nullopt;
    }
    return iter->second;
}

} // namespace asst::yys
