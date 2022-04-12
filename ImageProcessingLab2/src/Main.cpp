#include "MainHeader.hpp"

#include "HistogramVisualizer/HistogramVisualizer.hpp"
#include "HistogramModifier/LogHistogramModifier.hpp"
#include "ImagePreparer/NegativeContrastScalePreparer.hpp"

std::unique_ptr<argparse::ArgumentParser> InitMain(const int argc, char** argv) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    auto parser = std::make_unique<argparse::ArgumentParser>();
    parser->add_argument("-f", "--filepath")
        .required().help("specify the input image file"); 
    parser->add_argument("-l", "--logarithm")
        .nargs(1).scan<'i', int>().help("SAMPLE LOG TEXT");
    parser->add_argument("-p", "--prepare")
        .nargs(2).scan<'i', int>().help("SAMPLE PREP TEXT");

    try {
        parser->parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        throw err;
    }

    return parser;
}

int main(const int argc, char** argv) {
    try {
        auto parser   = InitMain(argc, argv);
        auto filePath = parser->get<std::string>("-f");
        auto image = std::make_shared<cv::Mat>(cv::imread(filePath, cv::ImreadModes::IMREAD_GRAYSCALE));
        //std::clog << std::format("Open Size: {} x {}\n", image->rows, image->cols);

        // HISTOGRAM WORK

        auto hist       = std::make_shared<ImageHistogram>(image);
        auto visualizer = std::make_unique<HistogramVisualizer>(hist);

        visualizer->PlotHistogram();
        cv::waitKey(0);

        cv::imshow("Before", *image);
        cv::waitKey(0);

        // LOG MOD
        if (parser->is_used("-l")) {
            const auto c = parser->get<int>("-l");

            std::unique_ptr<HistogramModifier> modifier = std::make_unique<LogHistogramModifier>(image, c); // TODO: var for const 25
            image = modifier->Modify();

            hist->SetImage(image);
            visualizer = std::make_unique<HistogramVisualizer>(hist);

            visualizer->PlotHistogram();
            cv::waitKey(0);

            cv::imshow("After", *image);
            cv::waitKey(0);

            cv::imwrite(std::format("log_{}", filePath), *image);
        }

        // PREP MOD
        if (parser->is_used("-p")) {
            const auto args = parser->get<std::vector<int>>("-p");

            std::unique_ptr<ImagePreparer> preparer = std::make_unique<NegativeContrastScalePreparer>(image, args.at(0), args.at(1));
            image = preparer->Prepare();

            hist->SetImage(image);
            visualizer = std::make_unique<HistogramVisualizer>(hist);

            visualizer->PlotHistogram();
            cv::waitKey(0);

            cv::imshow("After", *image);
            cv::waitKey(0);

            cv::imwrite(std::format("prep_{}", filePath), *image);

            //auto testMatrix = std::make_shared<cv::Mat>(256, 1, CV_8U, 0);
            //for (auto i = 0; i < testMatrix->rows; ++i) {
            //    testMatrix->at<uchar>(i, 0) = i;
            //}

            //std::unique_ptr<ImagePreparer> preparer = std::make_unique<NegativeContrastScalePreparer>(testMatrix, args.at(0), args.at(1));
            //std::cout << std::format("a = {}\nb = {}\n", args.at(0), args.at(1));

            //testMatrix = preparer->Prepare();
            //auto normMatrix = std::make_shared<cv::Mat>();
            //auto histImage = std::make_shared<cv::Mat>(200 + 2, 256 * 2, CV_8UC3, cv::Scalar(0.0, 0.0, 0.0));

            //cv::normalize(*testMatrix, *normMatrix, 0, histImage->rows - 2, cv::NORM_MINMAX, -1, cv::Mat());

            //for (auto i = 0; i < testMatrix->rows; ++i) {
            //    cv::rectangle(*histImage, cv::Rect(2 * i, histImage->rows - normMatrix->at<uchar>(i) - 2, 2, 2), cv::Scalar(0, 255, 0));
            //}

            //cv::imshow("test", *histImage);
            //cv::waitKey();
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

}
