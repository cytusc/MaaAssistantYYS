#pragma once

#include <optional>
#include <string>

#include "YYSTypes.h"

namespace asst::yys {

class ITemplateResolver {
public:
    virtual ~ITemplateResolver() = default;

    virtual std::optional<Rect> find_template(const std::string& name) const = 0;
};

} // namespace asst::yys
