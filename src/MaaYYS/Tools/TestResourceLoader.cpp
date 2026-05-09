#include <iostream>
#include <memory>

#include "Common/Resource/YYSResourceLoader.h"
#include "Base/YYSContext.h"

using namespace asst::yys;

int main()
{
    std::cout << "=== YYS Resource Loader Test ===" << std::endl;

    // 测试资源加载器
    const std::string resource_path = "D:/code/maa/MaaAssistantYYS/resource/YYS";

    std::cout << "Loading resources from: " << resource_path << std::endl;

    YYSResourceLoader loader;
    bool loaded = loader.load(resource_path);

    if (!loaded) {
        std::cerr << "❌ Failed to load resources" << std::endl;
        return 1;
    }

    std::cout << "✅ Resources loaded successfully" << std::endl;

    std::cout << "\n=== Found Templates ===" << std::endl;
    for (const auto& [id, resource] : loader.templates()) {
        std::cout << "  • " << id << ": " << resource.file.string() << std::endl;
        std::cout << "    ROI: [" << resource.roi.x << ", " << resource.roi.y << ", "
                  << resource.roi.width << ", " << resource.roi.height << "]" << std::endl;
        std::cout << "    Threshold: " << resource.threshold << std::endl;
        if (!resource.description.empty()) {
            std::cout << "    Description: " << resource.description << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "Total templates: " << loader.templates().size() << std::endl;
    std::cout << "Total OCR resources: " << loader.ocrs().size() << std::endl;

    // 测试特定模板
    std::cout << "\n=== Testing Specific Templates ===" << std::endl;
    const std::vector<std::string> test_templates = {
        "I_OROCHI",
        "I_OROCHI_FIRE",
        "I_REWARD",
        "O_LAYER_8"
    };

    for (const auto& template_id : test_templates) {
        auto templates = loader.templates();
        auto it = templates.find(template_id);
        if (it != templates.end()) {
            const auto& res = it->second;
            std::cout << "✅ " << template_id << " found" << std::endl;
            std::cout << "   File: " << res.file.string() << std::endl;

            // 检查文件是否存在
            if (std::filesystem::exists(res.file)) {
                std::cout << "   File exists: ✅" << std::endl;
            } else {
                std::cout << "   File exists: ❌ (missing)" << std::endl;
            }
        } else {
            std::cout << "❌ " << template_id << " not found" << std::endl;
        }
    }

    return 0;
}
