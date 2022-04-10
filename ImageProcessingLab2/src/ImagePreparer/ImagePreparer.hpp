#pragma once
#include "MainHeader.hpp"

struct ImagePreparer {
public:
    explicit ImagePreparer(const std::shared_ptr<cv::Mat>& image) : image(image) {

    }

    virtual ~ImagePreparer() = default;

    [[nodiscard]] virtual std::shared_ptr<cv::Mat> Prepare() = 0;

protected:
    std::shared_ptr<cv::Mat> image;
};