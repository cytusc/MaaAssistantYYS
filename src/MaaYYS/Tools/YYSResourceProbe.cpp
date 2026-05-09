#include <filesystem>
#include <iostream>

#include "Common/Resource/YYSResourceLoader.h"
#include "Common/Vision/MaaTemplateResolver.h"

int main(int argc, char** argv)
{
    using namespace asst::yys;

    const std::filesystem::path root = argc > 1 ? argv[1] : std::filesystem::path("../resource/YYS");

    YYSResourceLoader loader;
    if (!loader.load(root)) {
        std::cout << "resource_probe=load_failed\n";
        return 1;
    }

    MaaTemplateResolver resolver(loader);
    const auto orochi = resolver.find_template("I_OROCHI");
    const auto fire = resolver.find_template("I_OROCHI_FIRE");
    const bool layer = loader.ocrs().contains("O_LAYER");

    std::cout << "resource_probe=ok\n";
    std::cout << "templates=" << loader.templates().size() << '\n';
    std::cout << "ocrs=" << loader.ocrs().size() << '\n';
    std::cout << "I_OROCHI=" << (orochi ? "ok" : "missing") << '\n';
    std::cout << "I_OROCHI_FIRE=" << (fire ? "ok" : "missing") << '\n';
    std::cout << "O_LAYER=" << (layer ? "ok" : "missing") << '\n';

    return orochi && fire && layer ? 0 : 1;
}
