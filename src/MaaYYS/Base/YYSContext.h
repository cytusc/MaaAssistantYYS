#pragma once

#include <memory>

#include "Base/YYSConfig.h"

namespace asst::yys {

class IActionExecutor;
class ITemplateResolver;

class YYSContext {
public:
    YYSContext();

    std::shared_ptr<IActionExecutor> executor() const { return m_executor; }
    std::shared_ptr<ITemplateResolver> resolver() const { return m_resolver; }
    std::shared_ptr<YYSConfig> config() const { return m_config; }

    void set_executor(std::shared_ptr<IActionExecutor> executor) { m_executor = std::move(executor); }
    void set_resolver(std::shared_ptr<ITemplateResolver> resolver) { m_resolver = std::move(resolver); }
    void set_config(std::shared_ptr<YYSConfig> config) { m_config = std::move(config); }

private:
    std::shared_ptr<IActionExecutor> m_executor;
    std::shared_ptr<ITemplateResolver> m_resolver;
    std::shared_ptr<YYSConfig> m_config;
};

} // namespace asst::yys
