#include "NegativeContrastScalePreparer.hpp"

std::shared_ptr<cv::Mat> NegativeContrastScalePreparer::Prepare() {
    auto tempMat = std::make_shared<cv::Mat>(*this->image);

    //std::clog << std::format("Basic Size: {} x {}\n", this->image->rows, this->image->cols);
    for (auto i = 0; i < tempMat->rows; ++i) {
        for (auto j = 0; j < tempMat->cols; ++j) {
            tempMat->ptr(i, j)[0] = this->preparatorF(tempMat->ptr(i, j)[0]);
        }
    }
    //std::clog << std::format("Mod Size: {} x {}\n", tempMat->rows, tempMat->cols);


    return tempMat;
}
