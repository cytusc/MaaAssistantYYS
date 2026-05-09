#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

#include "Base/IOcrResolver.h"

namespace asst::yys {

class ScriptedOcrResolver : public IOcrResolver {
public:
    void set_sequence(std::string name, std::vector<std::optional<std::string>> sequence);

    std::optional<std::string> recognize(const std::string& ocr_id) const override;

private:
    struct SequenceState {
        std::vector<std::optional<std::string>> values;
        mutable std::size_t index = 0;
    };

    std::unordered_map<std::string, SequenceState> m_sequences;
};

} // namespace asst::yys
