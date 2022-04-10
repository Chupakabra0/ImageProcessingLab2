#pragma once
#include "ImagePreparer.hpp"
#include "../ImageHistogram/ImageHistogram.hpp"

class NegativeContrastScalePreparer : public ImagePreparer {
public:
    explicit NegativeContrastScalePreparer(const std::shared_ptr<cv::Mat>& image, int low = 0, int high = 255)
        : ImagePreparer(image), low(low), high(high),
          preparatorF([this](int f) {
              return f < this->low ? 1 : (f > this->high ? 0 : this->PrepareHelper(f));
          }) {
        this->minF = *std::min_element(this->image->begin<uchar>(), this->image->end<uchar>());
        this->maxF = *std::max_element(this->image->begin<uchar>(), this->image->end<uchar>());
    }

    virtual std::shared_ptr<cv::Mat> Prepare() override;

private:
    int low;
    int high;
    std::function<int(int)> preparatorF;

    // TEMPORARY
    int minF;
    int maxF;
    //

    [[nodiscard]] int PrepareHelper(int f) {
        //auto hist = std::make_unique<ImageHistogram>(this->image);
        /*const auto [minF, maxF] = std::make_pair(
            *std::min_element(this->image->begin<uchar>(), this->image->end<uchar>()) ,
            *std::max_element(this->image->begin<uchar>(), this->image->end<uchar>())
        );*/

        return 255 - (f - this->minF) * (this->high - this->low) / (this->maxF - this->minF) + this->minF;
    }
};