#include "ScriptedOcrResolver.h"

namespace asst::yys {

void ScriptedOcrResolver::set_sequence(std::string name, std::vector<std::optional<std::string>> sequence)
{
    m_sequences.insert_or_assign(std::move(name), SequenceState { .values = std::move(sequence) });
}

std::optional<std::string> ScriptedOcrResolver::recognize(const std::string& ocr_id) const
{
    const auto iter = m_sequences.find(ocr_id);
    if (iter == m_sequences.end() || iter->second.values.empty()) {
        return std::nullopt;
    }

    const auto& state = iter->second;
    const auto current = state.values[std::min(state.index, state.values.size() - 1)];
    if (state.index + 1 < state.values.size()) {
        ++state.index;
    }
    return current;
}

} // namespace asst::yys
