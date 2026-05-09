#include "YYSContext.h"

namespace asst::yys {

YYSContext::YYSContext() = default;

std::shared_ptr<YYSContext> YYSContext::create_with_defaults()
{
    auto ctx = std::make_shared<YYSContext>();
    ctx->set_config(YYSConfig::from_defaults());
    return ctx;
}

std::shared_ptr<YYSContext> YYSContext::create_from_config(const std::filesystem::path& config_path)
{
    auto ctx = std::make_shared<YYSContext>();
    ctx->set_config(YYSConfig::from_file(config_path));
    return ctx;
}

} // namespace asst::yys
