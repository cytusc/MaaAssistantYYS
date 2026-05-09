#pragma once

#include <string>
#include <memory>

#include "Base/YYSContext.h"

namespace asst::yys {

class OrochiLayerSelector {
public:
    explicit OrochiLayerSelector(std::shared_ptr<YYSContext> ctx);

    bool select(int layer_num);

private:
    bool find_layer_directly(int layer_num);
    bool swipe_to_find_layer(int target_layer);
    bool click_layer(const std::string& layer_target);

    std::shared_ptr<YYSContext> m_ctx;
    static constexpr int DEFAULT_TIMEOUT_MS = 3000;
};

} // namespace asst::yys
