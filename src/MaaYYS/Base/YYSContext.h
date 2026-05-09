#pragma once

#include <memory>

#include "Base/YYSConfig.h"

namespace asst::yys {

class IActionExecutor;
class ITemplateResolver;
class IOcrResolver;

class YYSContext {
public:
    YYSContext();

    static std::shared_ptr<YYSContext> create_with_defaults();
    static std::shared_ptr<YYSContext> create_from_config(const std::filesystem::path& config_path);

    std::shared_ptr<IActionExecutor> executor() const { return m_executor; }
    std::shared_ptr<ITemplateResolver> resolver() const { return m_resolver; }
    std::shared_ptr<IOcrResolver> ocr_resolver() const { return m_ocr_resolver; }
    std::shared_ptr<YYSConfig> config() const { return m_config; }

    void set_executor(std::shared_ptr<IActionExecutor> executor) { m_executor = std::move(executor); }
    void set_resolver(std::shared_ptr<ITemplateResolver> resolver) { m_resolver = std::move(resolver); }
    void set_ocr_resolver(std::shared_ptr<IOcrResolver> ocr_resolver) { m_ocr_resolver = std::move(ocr_resolver); }
    void set_config(std::shared_ptr<YYSConfig> config) { m_config = std::move(config); }

private:
    std::shared_ptr<IActionExecutor> m_executor;
    std::shared_ptr<ITemplateResolver> m_resolver;
    std::shared_ptr<IOcrResolver> m_ocr_resolver;
    std::shared_ptr<YYSConfig> m_config;
};

} // namespace asst::yys
