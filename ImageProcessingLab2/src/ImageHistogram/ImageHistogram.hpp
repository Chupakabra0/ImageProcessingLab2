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

    [[nodiscard]] int GetMaxColor() {
        return *std::max_element(this->image->begin<int>(), this->image->end<int>());
    }

    [[nodiscard]] int GetMinColor() {
        return *std::min_element(this->image->begin<int>(), this->image->end<int>());
    }

private:
    std::shared_ptr<cv::Mat> image;
};
