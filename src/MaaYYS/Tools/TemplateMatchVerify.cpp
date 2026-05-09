#include <iostream>
#include <string>
#include <filesystem>

#include <opencv2/imgcodecs.hpp>

#include "Common/Vision/MaaTemplateResolver.h"
#include "Common/Resource/YYSResourceLoader.h"
#include "Base/YYSTypes.h"

void print_usage(const char* program)
{
    std::cout << "Usage: " << program << " <screenshot_path> [resource_path] [template_id]" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "  screenshot_path  Path to the screenshot image file" << std::endl;
    std::cout << "  resource_path    Path to resource/YYS directory (default: D:/code/maa/MaaAssistantYYS/resource/YYS)" << std::endl;
    std::cout << "  template_id      Specific template ID to match (default: match all)" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << program << " screenshot.png" << std::endl;
    std::cout << "  " << program << " screenshot.png resource/YYS I_OROCHI" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    const std::string screenshot_path = argv[1];
    const std::string resource_path = (argc >= 3) ? argv[2] : "D:/code/maa/MaaAssistantYYS/resource/YYS";
    const std::string template_id = (argc >= 4) ? argv[3] : "";

    std::cout << "=== Template Match Verify ===" << std::endl;
    std::cout << "Screenshot: " << screenshot_path << std::endl;
    std::cout << "Resource:   " << resource_path << std::endl;
    std::cout << std::endl;

    cv::Mat screenshot = cv::imread(screenshot_path, cv::IMREAD_COLOR);
    if (screenshot.empty()) {
        std::cerr << "ERROR: Failed to load screenshot: " << screenshot_path << std::endl;
        return 1;
    }
    std::cout << "Screenshot loaded: " << screenshot.cols << "x" << screenshot.rows
              << " channels=" << screenshot.channels() << std::endl;

    asst::yys::YYSResourceLoader loader;
    if (!loader.load(resource_path)) {
        std::cerr << "ERROR: Failed to load resources from: " << resource_path << std::endl;
        return 1;
    }
    std::cout << "Resources loaded: " << loader.templates().size() << " templates" << std::endl;
    std::cout << std::endl;

    asst::yys::MaaTemplateResolver resolver(loader);
    resolver.set_image(screenshot);

    if (!template_id.empty()) {
        auto result = resolver.find_template(template_id);
        std::cout << "Template: " << template_id << std::endl;
        if (result) {
            std::cout << "  MATCH: x=" << result->x << " y=" << result->y
                      << " w=" << result->width << " h=" << result->height << std::endl;
        }
        else {
            std::cout << "  NO MATCH" << std::endl;
        }
    }
    else {
        std::cout << "=== Matching All Templates ===" << std::endl;
        int matched = 0;
        int total = 0;

        for (const auto& [id, res] : loader.templates()) {
            ++total;
            auto result = resolver.find_template(id);
            if (result) {
                ++matched;
                std::cout << "  ✅ " << id
                          << " -> x=" << result->x << " y=" << result->y
                          << " w=" << result->width << " h=" << result->height
                          << " (" << res.description << ")" << std::endl;
            }
            else {
                std::cout << "  ❌ " << id
                          << " -> no match (" << res.description << ")" << std::endl;
            }
        }

        std::cout << std::endl;
        std::cout << "=== Summary ===" << std::endl;
        std::cout << "Matched: " << matched << "/" << total << std::endl;
    }

    return 0;
}
