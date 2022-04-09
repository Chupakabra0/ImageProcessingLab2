#pragma once
#include "MainHeader.hpp"
#include "../src/ImageHistogram/ImageHistogram.hpp"

class HistogramVisualizer {
public:
    explicit HistogramVisualizer(const std::shared_ptr<ImageHistogram>& hist) : hist(hist) {

    }

    ~HistogramVisualizer() = default;

    void PlotHistogram() {
        // Calculate histogram
        const auto temp = hist->GetHistogram();

        // Set future output image
        auto histogramImage = std::make_unique<cv::Mat>(200, 256 * 2, CV_8UC3, cv::Scalar(0, 0, 0));
        // Set output matrxi for normalize
        auto normalizedHist = std::make_unique<cv::Mat>();

        cv::normalize(*temp, *normalizedHist, 0, histogramImage->rows, cv::NORM_MINMAX, -1, cv::Mat());

        for (auto i = 0; i < temp->rows; ++i) {
            cv::rectangle(*histogramImage, cv::Rect(2 * i, histogramImage->rows - normalizedHist->at<double>(i, 0), 2, normalizedHist->at<double>(i, 0)), cv::Scalar(0, 0, 255));
        }

        cv::imshow("Histogram", *histogramImage);
    }

private:
    std::shared_ptr<ImageHistogram> hist;
};