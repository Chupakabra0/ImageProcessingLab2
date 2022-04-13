# Лабораторная работа №2

## Постановка задачи

Выберите одну из фотографий. Превратить её в полутоновое изображение. Сгенерировать два изображения: затемнённое и освещённое.

Разработать приложение, в котором будут выполняться следующие действия с загруженным полутоновым изображением:

- расчёт и построение гистограммы;
- выравнивание гистограммы изображения, метод согласно **варианту 16**;
- реализовать один из метод препарирования полутонового изображения  согласно **варианту 16**.

**Вариант 16:**

Для выравнивания гистограммы реализовать *логарифмическую коррекцию*. Для препарирования - метод линейного контрастирования (негатив).

## Выполнение первой части задания

Сгенерируем изображение в оттенках серого, а также его затемнённую и осветлённую версию.

![monochrome](https://i.ibb.co/CQBFpGg/monochrome.png)
![dark](https://i.ibb.co/Hr5hXmP/dark.png)
![light](https://i.ibb.co/8cQSxXG/light.png)

## Выполнение второй части задания

Программа написана на языке C++ с использованием библиотеки OpenCV. Код программы приложен ниже:

```cpp
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
    catch (const std::exception&) {
        throw;
    }

    return parser;
}

int main(const int argc, char** argv) {
    try {
        auto parser   = InitMain(argc, argv);
        auto filePath = parser->get<std::string>("-f");
        auto image = std::make_shared<cv::Mat>(cv::imread(filePath, cv::ImreadModes::IMREAD_GRAYSCALE));

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
            const auto modImage = modifier->Modify();

            hist->SetImage(modImage);
            visualizer = std::make_unique<HistogramVisualizer>(hist);

            visualizer->PlotHistogram();
            cv::waitKey(0);

            cv::imshow("After", *modImage);
            cv::waitKey(0);

            cv::imwrite(std::format("log_{}_{}", c, filePath), *modImage);
        }

        // PREP MOD
        if (parser->is_used("-p")) {
            const auto args = parser->get<std::vector<int>>("-p");

            std::unique_ptr<ImagePreparer> preparer = std::make_unique<NegativeContrastScalePreparer>(image, args.at(0), args.at(1));
            const auto modImage = preparer->Prepare();

            hist->SetImage(modImage);
            visualizer = std::make_unique<HistogramVisualizer>(hist);

            visualizer->PlotHistogram();
            cv::waitKey(0);

            cv::imshow("After", *modImage);
            cv::waitKey(0);

            cv::imwrite(std::format("prep_{}_{}_{}", args.at(0), args.at(1), filePath), *modImage);
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

}

```

При запуске приложения с параметром `--help` приложение выведет справку.

```
Usage: ImageProcessingLab2.exe [options]

Optional arguments:
-h --help       shows help message and exits
-v --version    prints version information and exits
-f --filepath   specify the input image file [required]
-l --logarithm  SAMPLE LOG TEXT
-p --prepare    SAMPLE PREP TEXT
```

Запуск с аргументом `--logarithm  ` и числовым параметром запустит логарифмическое преобразование изображения. Пример работы программы с входными строками ``ImageProcessingLab2.exe -f dark.png -l 25`` и  ``ImageProcessingLab2.exe -f dark.png -l 50``

![input](https://i.ibb.co/Hr5hXmP/dark.png)
![histInput](https://i.ibb.co/k0XNrBw/1.png)
![output25](https://i.ibb.co/82ypqvD/log-25-dark.png)
![outputHist25](https://i.ibb.co/Jj4kn0M/2.png)
![output50](https://i.ibb.co/RvWxVW1/log-50-dark.png)
![outputHist50](https://i.ibb.co/rkHhW21/3.png)

Запуск с аргументом `--prepare` и числовыми параметрами запустит препарирование входного изображение методом линейного контрастирования. Пример работы программы с входными строками ``ImageProcessingLab2.exe -f dark.png -p 50 150`` и ``ImageProcessingLab2.exe -f dark.png -p 200 255``

![input](https://i.ibb.co/Hr5hXmP/dark.png)
![histInput](https://i.ibb.co/k0XNrBw/1.png)
![output50_150](https://i.ibb.co/crfXD2D/prep-50-150-dark.png)
![outputHist50_150](https://i.ibb.co/FmzYTFH/4.png)
![output200_255](https://i.ibb.co/cC9xV5j/prep-200-255-dark.png)
![outputHist200_255](https://i.ibb.co/Dzsj5rB/5.png)

## Выводы:

Было разработано ПО для отображения и коррекции гистограммы изображения в оттенках серого, рассмотрены различные методы препарирования этих изображений.

Логарифмическое выравнивание гистограммы изображения, как и предполагалось, помогают осветить фото, а линейная коррекция - выделить некоторые элементы.