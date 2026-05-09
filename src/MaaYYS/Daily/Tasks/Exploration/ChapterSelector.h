#pragma once

#include <string>
#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class ChapterSelector {
public:
    explicit ChapterSelector(std::shared_ptr<YYSContext> ctx);

    bool select(int chapter_num);

private:
    bool find_chapter_directly();
    bool swipe_to_find_chapter(int target_chapter);
    bool click_chapter();

    std::shared_ptr<YYSContext> m_ctx;
    static constexpr int DEFAULT_TIMEOUT_MS = 3000;
};

} // namespace asst::yys
