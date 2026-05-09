#pragma once

#include <functional>
#include <optional>
#include <memory>
#include <vector>

#include "Base/ITemplateResolver.h"
#include "Common/Resource/YYSResourceLoader.h"

#ifdef MAAYYS_WITH_OPENCV
#include <opencv2/core.hpp>
#endif

namespace asst::yys {

class MaaTemplateResolver : public ITemplateResolver {
public:
    using MatchFn = std::function<std::optional<Rect>(const TemplateResource&)>;

    MaaTemplateResolver() = default;
    explicit MaaTemplateResolver(const YYSResourceLoader& loader);
    MaaTemplateResolver(const YYSResourceLoader& loader, MatchFn match);

    void set_resources(const YYSResourceLoader& loader);
    void bind_matcher(MatchFn match);

#ifdef MAAYYS_WITH_OPENCV
    void set_image(const cv::Mat& image);
    void set_image_from_raw(const void* data, int width, int height, int channels = 4);
#else
    void set_image(const void* image_data, int width = 0, int height = 0, int channels = 4);
#endif

    void clear_image();

    std::optional<Rect> find_template(const std::string& name) const override;

private:
    std::optional<Rect> match_with_maa(const TemplateResource& resource) const;

#ifdef MAAYYS_WITH_OPENCV
    std::optional<Rect> match_template_opencv(const TemplateResource& resource) const;
#endif

    std::optional<Rect> fallback_to_roi(const TemplateResource& resource) const;

private:
    std::unordered_map<std::string, TemplateResource> m_templates;
    MatchFn m_match;

#ifdef MAAYYS_WITH_OPENCV
    cv::Mat m_current_image;
#else
    const void* m_current_image = nullptr;
    int m_image_width = 0;
    int m_image_height = 0;
    int m_image_channels = 4;
#endif

    static constexpr int DEFAULT_BASE_WIDTH = 1280;
    static constexpr int DEFAULT_BASE_HEIGHT = 720;
};

} // namespace asst::yys
