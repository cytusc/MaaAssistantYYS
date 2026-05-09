#pragma once
#include <memory>
#include <string>

namespace asst::yys {

class YYSContext;  // 前向声明

class YYSModule {
public:
    virtual ~YYSModule() = default;
    virtual std::string name() const = 0;
    virtual bool init(std::shared_ptr<YYSContext> ctx);
    
protected:
    std::shared_ptr<YYSContext> m_ctx;
};

} // namespace asst::yys
