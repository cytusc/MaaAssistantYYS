#pragma once

#include <optional>
#include <string>

namespace asst::yys {

class IOcrResolver {
public:
    virtual ~IOcrResolver() = default;
    virtual std::optional<std::string> recognize(const std::string& ocr_id) const = 0;
};

} // namespace asst::yys
