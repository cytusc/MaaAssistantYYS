#include "MaaTemplateResolver.h"

#include <iostream>

#ifdef MAAYYS_WITH_OPENCV
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#endif

namespace asst::yys {

MaaTemplateResolver::MaaTemplateResolver(const YYSResourceLoader& loader)
{
    set_resources(loader);
}

MaaTemplateResolver::MaaTemplateResolver(const YYSResourceLoader& loader, MatchFn match)
    : m_match(std::move(match))
{
    set_resources(loader);
}

void MaaTemplateResolver::set_resources(const YYSResourceLoader& loader)
{
    m_templates = loader.templates();
}

void MaaTemplateResolver::bind_matcher(MatchFn match)
{
    m_match = std::move(match);
}

#ifdef MAAYYS_WITH_OPENCV

void MaaTemplateResolver::set_image(const cv::Mat& image)
{
    if (image.empty()) {
        m_current_image.release();
        return;
    }

    if (image.cols != DEFAULT_BASE_WIDTH || image.rows != DEFAULT_BASE_HEIGHT) {
        cv::resize(image, m_current_image, cv::Size(DEFAULT_BASE_WIDTH, DEFAULT_BASE_HEIGHT));
    }
    else {
        m_current_image = image.clone();
    }

    if (m_current_image.channels() == 4) {
        cv::cvtColor(m_current_image, m_current_image, cv::COLOR_BGRA2BGR);
    }
    else if (m_current_image.channels() == 1) {
        cv::cvtColor(m_current_image, m_current_image, cv::COLOR_GRAY2BGR);
    }
}

void MaaTemplateResolver::set_image_from_raw(const void* data, int width, int height, int channels)
{
    if (!data || width <= 0 || height <= 0) {
        m_current_image.release();
        return;
    }

    int cv_type = CV_8UC1;
    if (channels == 3) cv_type = CV_8UC3;
    else if (channels == 4) cv_type = CV_8UC4;

    cv::Mat raw(height, width, cv_type, const_cast<void*>(data));
    set_image(raw);
}

#else

void MaaTemplateResolver::set_image(const void* image_data, int width, int height, int channels)
{
    m_current_image = image_data;
    m_image_width = width;
    m_image_height = height;
    m_image_channels = channels;
}

#endif

void MaaTemplateResolver::clear_image()
{
#ifdef MAAYYS_WITH_OPENCV
    m_current_image.release();
#else
    m_current_image = nullptr;
    m_image_width = 0;
    m_image_height = 0;
#endif
}

std::optional<Rect> MaaTemplateResolver::find_template(const std::string& name) const
{
    const auto iter = m_templates.find(name);
    if (iter == m_templates.end()) {
        return std::nullopt;
    }

    if (m_match) {
        return m_match(iter->second);
    }

    auto result = match_with_maa(iter->second);
    if (result) {
        return result;
    }

    return fallback_to_roi(iter->second);
}

std::optional<Rect> MaaTemplateResolver::match_with_maa(const TemplateResource& resource) const
{
#ifdef MAAYYS_WITH_OPENCV
    return match_template_opencv(resource);
#else
    (void)resource;
    return std::nullopt;
#endif
}

#ifdef MAAYYS_WITH_OPENCV

std::optional<Rect> MaaTemplateResolver::match_template_opencv(const TemplateResource& resource) const
{
    if (m_current_image.empty()) {
        return std::nullopt;
    }

    cv::Mat templ = cv::imread(resource.file.string(), cv::IMREAD_COLOR);
    if (templ.empty()) {
        std::cerr << "[WARN] Template image not found or unreadable: " << resource.file << std::endl;
        return std::nullopt;
    }

    cv::Mat search_region = m_current_image;
    cv::Rect search_rect(0, 0, m_current_image.cols, m_current_image.rows);

    if (!resource.roi.empty()
        && resource.roi.x >= 0 && resource.roi.y >= 0
        && resource.roi.x + resource.roi.width <= m_current_image.cols
        && resource.roi.y + resource.roi.height <= m_current_image.rows) {
        search_rect = cv::Rect(resource.roi.x, resource.roi.y, resource.roi.width, resource.roi.height);
        search_region = m_current_image(search_rect);
    }

    if (templ.cols > search_region.cols || templ.rows > search_region.rows) {
        std::cerr << "[WARN] Template " << resource.id
                  << " (" << templ.cols << "x" << templ.rows << ")"
                  << " larger than search region ("
                  << search_region.cols << "x" << search_region.rows << ")" << std::endl;
        return std::nullopt;
    }

    cv::Mat result_mat;
    int match_method = cv::TM_CCOEFF_NORMED;
    cv::matchTemplate(search_region, templ, result_mat, match_method);

    double min_val = 0, max_val = 0;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(result_mat, &min_val, &max_val, &min_loc, &max_loc);

    double score = max_val;
    cv::Point match_loc = max_loc;

    if (score < resource.threshold) {
        return std::nullopt;
    }

    Rect matched;
    matched.x = search_rect.x + match_loc.x;
    matched.y = search_rect.y + match_loc.y;
    matched.width = templ.cols;
    matched.height = templ.rows;

    return matched;
}

#endif

std::optional<Rect> MaaTemplateResolver::fallback_to_roi(const TemplateResource& resource) const
{
    if (resource.roi.empty()) {
        return std::nullopt;
    }
    return std::optional<Rect>(resource.roi);
}

} // namespace asst::yys
