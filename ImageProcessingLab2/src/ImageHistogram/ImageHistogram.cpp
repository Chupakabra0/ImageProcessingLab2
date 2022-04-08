#include "ImageHistogram.hpp"

std::shared_ptr<cv::Mat> ImageHistogram::GetImage() {
    return this->image;
}

void ImageHistogram::SetImage(const std::shared_ptr<cv::Mat>& image) {
    this->image = image;
}

[[nodiscard]] std::unique_ptr<cv::Mat> ImageHistogram::GetHistogram() {
    auto tempHist = std::make_unique<cv::Mat>(cv::Mat::zeros(256, 1, CV_64F));
    auto tempImage = std::make_unique<cv::Mat>(*this->image);

    tempImage->convertTo(*tempImage, CV_64F);

    for (auto i = 0; i < tempImage->rows; ++i) {
        for (auto j = 0; j < tempImage->cols; ++j) {
            const auto value = tempImage->at<double>(i, j);
            ++tempHist->at<double>(value, 0);
        }
    }

    return tempHist;
}
