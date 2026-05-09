#pragma once

#include <memory>
#include <string>

namespace asst::yys {

class YYSContext;

class YYSFlowBase {
public:
    virtual ~YYSFlowBase() = default;

    virtual bool init(std::shared_ptr<YYSContext> ctx);
    virtual bool run() = 0;
    virtual std::string name() const { return m_name; }

    void set_next(std::unique_ptr<YYSFlowBase> next);

protected:
    explicit YYSFlowBase(std::string name) : m_name(std::move(name)) {}

    std::string m_name;
    std::shared_ptr<YYSContext> m_ctx;
    std::unique_ptr<YYSFlowBase> m_next;
};

} // namespace asst::yys
