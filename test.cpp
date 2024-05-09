
#include <catch.hpp>
#include "parser.h"

// NOLINTBEGIN
std::string CheckInvalidArgs(std::vector<std::string> str_vec) {
    Parser parser;
    std::string res;
    char* args[str_vec.size()];
    for (size_t i = 0; i < str_vec.size(); ++i) {
        args[i] = str_vec[i].data();
    }

    try {
        parser.Parse(static_cast<int>(str_vec.size()), args);
    } catch (const std::runtime_error& e) {
        res = e.what();
    }
    return res;
}

TEST_CASE("parser") {
    Parser parser;
    std::string str;
    try {
        parser.Parse(0, {});
    } catch (const std::runtime_error& e) {
        str = e.what();
    }

    std::vector<std::string> args = {};
    REQUIRE(CheckInvalidArgs(args) == "Not enough parameters");

    args = {"./image_processor"};
    REQUIRE(CheckInvalidArgs(args) == "Not enough parameters");

    args = {"./image_processor", "lenna.bmp"};
    REQUIRE(CheckInvalidArgs(args) == "Not enough parameters");

    args = {"./image_processor", "lenna.bmp", "lenna.bmp"};
    REQUIRE(CheckInvalidArgs(args) == "");

    args = {"./image_processor", "lenna.bmp", "lenna.bmp", "-gs"};
    REQUIRE(CheckInvalidArgs(args) == "");

    args = {"./image_processor", "lenna.bmp", "lenna.bmp", "10"};
    REQUIRE(CheckInvalidArgs(args) == "Incorrect parameters");

    args = {"./image_processor", "lenna.bmp", "lenna.bmp", "crop"};
    REQUIRE(CheckInvalidArgs(args) == "Incorrect parameters");

    args = {"./image_processor", "lenna.bmp", "lenna.bmp", "-crop"};
    REQUIRE(CheckInvalidArgs(args) == "Incorrect parameters");

    args = {"./image_processor", "lenna.bmp", "lenna.bmp", "-blur", "aaa"};
    REQUIRE(CheckInvalidArgs(args) == "Incorrect parameters");

    args = {"./image_processor", "lenna.bmp", "lenna.bmp", "-blur", "10", "-gs", "-crop", "600", "800"};
    REQUIRE(CheckInvalidArgs(args) == "");

    args = {"./image_processor", "lenna.bmp", "lenna.bmp", "-sharp", "10"};
    REQUIRE(CheckInvalidArgs(args) == "Incorrect parameters");
}

std::string CheckInvalidInputPath(std::string str) {
    Image image;
    std::string res;
    char* path = str.data();
    try {
        image.Read(path);
    } catch (const std::runtime_error& e) {
        res = e.what();
    }
    return res;
}

std::string CheckInvalidOutputPath(std::string str) {
    Image image;
    std::string res;
    char* path = str.data();
    try {
        image.Write(path);
    } catch (const std::runtime_error& e) {
        res = e.what();
    }
    return res;
}

TEST_CASE("image") {
    std::string path = "";
    REQUIRE(CheckInvalidInputPath(path) == "File could not be opened");
    REQUIRE(CheckInvalidOutputPath(path) == "File could not be opened");

    path = "/home/marya/pmi-2212-2-Maria-Prokhorova-Maria/tasks/image_processor/test_script/data/flag.bmp";
    REQUIRE(CheckInvalidInputPath(path) == "");

    path = "/home/marya/pmi-2212-2-Maria-Prokhorova-Maria/tasks/image_processor/test_script/data/lenna.bmp";
    REQUIRE(CheckInvalidOutputPath(path) == "");

    path = "lenna.png";
    REQUIRE(CheckInvalidInputPath(path) == "File could not be opened");
    REQUIRE(CheckInvalidOutputPath(path) == "File could not be opened");
}

TEST_CASE("negative") {
    const int size = 20;
    Image image(size, size);
    Image right_image(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Color color = {0, 0, 0};
            image.SetColor(color, i, j);
            color = {1, 1, 1};
            right_image.SetColor(color, i, j);
        }
    }
    Negative negative;
    negative.Apply(image);
    const double e = 1e-6;
    bool check = true;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (fabs(static_cast<double>(right_image.GetColor(i, j).red - image.GetColor(i, j).red)) > e ||
                fabs(static_cast<double>(right_image.GetColor(i, j).green - image.GetColor(i, j).green)) > e ||
                fabs(static_cast<double>(right_image.GetColor(i, j).blue - image.GetColor(i, j).blue)) > e) {
                check = false;
            }
        }
    }
    REQUIRE(check);
}

TEST_CASE("grayscale") {
    const int size = 20;
    const float coef_r = 0.299;
    const float coef_g = 0.587;
    const float coef_b = 0.114;
    Image image(size, size);
    Image right_image(size, size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Color color = {0.8, 0.8, 0.8};
            image.SetColor(color, i, j);
            float col = 0.8 * coef_r + 0.8 * coef_g + 0.8 * coef_b;
            color = {col, col, col};
            right_image.SetColor(color, i, j);
        }
    }
    Grayscale grayscale;
    grayscale.Apply(image);
    const double e = 1e-6;
    bool check = true;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (fabs(static_cast<double>(right_image.GetColor(i, j).red - image.GetColor(i, j).red)) > e ||
                fabs(static_cast<double>(right_image.GetColor(i, j).green - image.GetColor(i, j).green)) > e ||
                fabs(static_cast<double>(right_image.GetColor(i, j).blue - image.GetColor(i, j).blue)) > e) {
                check = false;
            }
        }
    }
    REQUIRE(check);
}
// NOLINTEND
