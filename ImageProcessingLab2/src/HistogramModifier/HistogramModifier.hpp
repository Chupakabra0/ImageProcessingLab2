#pragma once
#include "../ImageHistogram/ImageHistogram.hpp"

struct HistogramModifier {
public:
    explicit HistogramModifier(const std::shared_ptr<cv::Mat>& image) : image(image) {

    }

    virtual ~HistogramModifier() = default;

    [[nodiscard]] virtual std::shared_ptr<cv::Mat> Modify() = 0;

protected:
    std::shared_ptr<cv::Mat> image;
};
