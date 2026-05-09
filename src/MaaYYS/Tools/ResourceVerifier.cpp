#include <iostream>
#include <filesystem>

#include "Common/Resource/YYSResourceLoader.h"

using namespace asst::yys;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: ResourceVerifier <resource_dir>" << std::endl;
        return 1;
    }

    const std::filesystem::path resource_dir(argv[1]);

    if (!std::filesystem::exists(resource_dir)) {
        std::cerr << "Resource directory does not exist: " << resource_dir << std::endl;
        return 1;
    }

    std::cout << "Loading resources from: " << resource_dir << std::endl;

    YYSResourceLoader loader;
    if (!loader.load(resource_dir)) {
        std::cerr << "Failed to load resources" << std::endl;
        return 1;
    }

    std::cout << "\n=== Templates ===" << std::endl;
    for (const auto& [id, resource] : loader.templates()) {
        std::cout << "  - " << id << ": " << resource.file.string()
                  << " [threshold: " << resource.threshold << "]"
                  << " " << resource.description << std::endl;
    }

    std::cout << "\n=== OCR Resources ===" << std::endl;
    for (const auto& [id, resource] : loader.ocrs()) {
        std::cout << "  - " << id << ": " << resource.mode << std::endl;
    }

    std::cout << "\nTotal templates: " << loader.templates().size() << std::endl;
    std::cout << "Total OCR resources: " << loader.ocrs().size() << std::endl;

    return 0;
}
