#pragma once
#include "HistogramModifier.hpp"

class LogHistogramModifier : public HistogramModifier {
public:
    explicit LogHistogramModifier(const std::shared_ptr<cv::Mat>& hist, int c)
        : HistogramModifier(hist), c(c),
          correctionF([this](int f) {
                return static_cast<int>(this->c * log(1 + f));
          }) {
    }

    ~LogHistogramModifier() override = default;

    virtual std::shared_ptr<cv::Mat> Modify() override;

private:
    int c;
    std::function<int(int)> correctionF;
};
