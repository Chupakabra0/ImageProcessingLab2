#include "MainHeader.hpp"

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
        auto mat      = std::make_unique<cv::Mat>(cv::imread(filePath, cv::ImreadModes::IMREAD_REDUCED_GRAYSCALE_8));

        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

}
