#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Base/ITemplateResolver.h"

namespace asst::yys {

class ScriptedTemplateResolver : public ITemplateResolver {
public:
    void set_sequence(std::string name, std::vector<std::optional<Rect>> sequence);

    std::optional<Rect> find_template(const std::string& name) const override;

private:
    struct SequenceState {
        std::vector<std::optional<Rect>> values;
        mutable std::size_t index = 0;
    };

    std::unordered_map<std::string, SequenceState> m_sequences;
};

} // namespace asst::yys
