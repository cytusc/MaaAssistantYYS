#pragma once
#include "Base/YYSModule.h"
#include "Base/IYYSActivity.h"

namespace asst::yys {

class YYSBaseActivity : public YYSModule, public IYYSActivity {
public:
    bool init(std::shared_ptr<YYSContext> ctx) override;
    bool run() override;
    
protected:
    virtual bool _run() = 0;
    virtual bool on_start();
    virtual bool on_end();
};

} // namespace asst::yys
