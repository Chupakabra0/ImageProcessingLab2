#include "MainHeader.hpp"

#include "HistogramVisualizer/HistogramVisualizer.hpp"
#include "HistogramModifier/LogHistogramModifier.hpp"

std::unique_ptr<argparse::ArgumentParser> InitMain(const int argc, char** argv) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    auto parser = std::make_unique<argparse::ArgumentParser>();
    parser->add_argument("-f", "--filepath")
        .required().help("specify the input image file");

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
        //auto image    = std::make_shared<cv::Mat>(cv::imread(filePath, cv::ImreadModes::IMREAD_ANYCOLOR | cv::ImreadModes::IMREAD_ANYDEPTH));
        std::clog << std::format("Open Size: {} x {}\n", image->rows, image->cols);

        // HISTOGRAM WORK

        auto hist       = std::make_shared<ImageHistogram>(image);
        auto visualizer = std::make_unique<HistogramVisualizer>(hist);

        visualizer->PlotHistogram();
        cv::waitKey(0);

        cv::imshow("Before", *image);
        cv::waitKey(0);

        // LOG MOD

        std::unique_ptr<HistogramModifier> modifier = std::make_unique<LogHistogramModifier>(image, 25); // TODO: var for const 25
        image = modifier->Modify();

        hist->SetImage(image);
        visualizer = std::make_unique<HistogramVisualizer>(hist);

        visualizer->PlotHistogram();
        cv::waitKey(0);

        cv::imshow("After", *image);
        cv::waitKey(0);

        cv::imwrite(std::format("mod_{}", filePath), *image);

        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

}
