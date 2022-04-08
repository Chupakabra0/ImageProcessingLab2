#pragma once
#include "MainHeader.hpp"

class ImageHistogram {
public:
    explicit ImageHistogram(const std::shared_ptr<cv::Mat>& image) : image(image) {

    }

    ~ImageHistogram() = default;

    [[nodiscard]] std::shared_ptr<cv::Mat> GetImage();

    void SetImage(const std::shared_ptr<cv::Mat>& image);

    [[nodiscard]] std::unique_ptr<cv::Mat> GetHistogram();

private:
    std::shared_ptr<cv::Mat> image;
};
