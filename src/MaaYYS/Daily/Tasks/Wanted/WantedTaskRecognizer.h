#pragma once

#include <memory>
#include <string>

#include "Base/YYSContext.h"

namespace asst::yys {

class WantedTaskRecognizer {
public:
    explicit WantedTaskRecognizer(std::shared_ptr<YYSContext> ctx);

    bool find_and_accept_task();
    const std::string& recognized_text() const { return m_recognized_text; }

private:
    std::shared_ptr<YYSContext> m_ctx;
    std::string m_recognized_text;
    static constexpr int DEFAULT_TIMEOUT_MS = 5000;
};

} // namespace asst::yys
