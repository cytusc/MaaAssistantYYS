#include "ChapterSelector.h"

#include <thread>
#include <chrono>

#include "Base/ITemplateResolver.h"
#include "Base/IActionExecutor.h"
#include "Base/YYSTypes.h"
#include "Common/Logger/YYSLogger.h"
#include "Common/Flow/YYSWaitFlow.h"
#include "Common/Flow/YYSActionFlow.h"
#include "Common/Flow/YYSRetryFlow.h"

namespace asst::yys {

ChapterSelector::ChapterSelector(std::shared_ptr<YYSContext> ctx)
    : m_ctx(std::move(ctx))
{
}

bool ChapterSelector::select(int chapter_num)
{
    if (!m_ctx) {
        YYS_LOG_ERROR("Context not available");
        return false;
    }

    YYS_LOG_INFO("Selecting chapter: %d", chapter_num);

    if (find_chapter_directly()) {
        YYS_LOG_INFO("Chapter found directly");
        return click_chapter();
    }

    YYS_LOG_INFO("Chapter not found directly, trying to swipe...");
    if (swipe_to_find_chapter(chapter_num)) {
        return true;
    }

    YYS_LOG_ERROR("Failed to select chapter: %d", chapter_num);
    return false;
}

bool ChapterSelector::find_chapter_directly()
{
    YYSWaitFlow wait_flow("I_CHAPTER", DEFAULT_TIMEOUT_MS);
    wait_flow.init(m_ctx);
    return wait_flow.run();
}

bool ChapterSelector::swipe_to_find_chapter(int target_chapter)
{
    if (!m_ctx || !m_ctx->executor()) {
        YYS_LOG_ERROR("Executor not available for swipe");
        return false;
    }

    YYS_LOG_INFO("Swiping to find chapter: %d", target_chapter);

    // Swipe down to find later chapters (higher numbers are lower in the list)
    const bool swipe_down = target_chapter > 15;

    for (int i = 0; i < 5; ++i) {
        if (find_chapter_directly()) {
            YYS_LOG_INFO("Found chapter after %d swipes", i);
            return click_chapter();
        }

        YYSActionFlow swipe_flow(
            ActionType::Swipe,
            640, swipe_down ? 520 : 240,
            640, swipe_down ? 240 : 520);
        swipe_flow.init(m_ctx);
        swipe_flow.run();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    YYS_LOG_WARN("Chapter %d not found after swiping", target_chapter);
    return false;
}

bool ChapterSelector::click_chapter()
{
    YYSRetryFlow click_flow(
        std::make_unique<YYSActionFlow>(ActionType::Click, 0, 0, "I_CHAPTER"),
        3);
    click_flow.init(m_ctx);
    if (!click_flow.run()) {
        YYS_LOG_ERROR("Failed to click chapter");
        return false;
    }
    YYS_LOG_INFO("Clicked chapter successfully");
    return true;
}

} // namespace asst::yys
