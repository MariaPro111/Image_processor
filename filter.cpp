
#include <iostream>
#include <cmath>
#include "filter.h"

Crop::Crop(int width, int height) : width_(width), height_(height) {
}

Image Crop::Apply(Image &image) {
    int new_width = std::min(image.GetWidth(), width_);
    int new_height = std::min(image.GetHeight(), height_);
    Image new_image(new_width, new_height);
    Color new_color;
    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            new_color = image.GetColor(image.GetHeight() - y - 1, x);
            new_image.SetColor(new_color, new_height - y - 1, x);
        }
    }
    return new_image;
}

Image Grayscale::Apply(Image &image) {
    const float coef_r = 0.299;
    const float coef_g = 0.587;
    const float coef_b = 0.114;
    Color new_color;
    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            new_color.red = coef_r * image.GetColor(y, x).red + coef_g * image.GetColor(y, x).green +
                            coef_b * image.GetColor(y, x).blue;
            new_color.green = new_color.red;
            new_color.blue = new_color.red;
            image.SetColor(new_color, y, x);
        }
    }
    return image;
}

Image Negative::Apply(Image &image) {
    Color new_color;
    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            new_color.red = 1 - image.GetColor(y, x).red;
            new_color.green = 1 - image.GetColor(y, x).green;
            new_color.blue = 1 - image.GetColor(y, x).blue;
            image.SetColor(new_color, y, x);
        }
    }
    return image;
}

Image Sharpening::Apply(Image &image) {
    const int arg = 5;
    Color new_color;
    Image new_image(image.GetWidth(), image.GetHeight());
    int new_x = 0;
    int new_y = 0;
    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            new_y = std::max(0, y - 1);
            new_x = x;
            new_color.red = -1 * image.GetColor(new_y, new_x).red;
            new_color.green = -1 * image.GetColor(new_y, new_x).green;
            new_color.blue = -1 * image.GetColor(new_y, new_x).blue;

            new_y = std::min(image.GetHeight() - 1, y + 1);
            new_color.red -= image.GetColor(new_y, new_x).red;
            new_color.green -= image.GetColor(new_y, new_x).green;
            new_color.blue -= image.GetColor(new_y, new_x).blue;

            new_x = std::max(0, x - 1);
            new_y = y;
            new_color.red -= image.GetColor(new_y, new_x).red;
            new_color.green -= image.GetColor(new_y, new_x).green;
            new_color.blue -= image.GetColor(new_y, new_x).blue;

            new_x = std::min(image.GetWidth() - 1, x + 1);
            new_color.red -= image.GetColor(new_y, new_x).red;
            new_color.green -= image.GetColor(new_y, new_x).green;
            new_color.blue -= image.GetColor(new_y, new_x).blue;

            new_color.red += arg * image.GetColor(y, x).red;
            new_color.green += arg * image.GetColor(y, x).green;
            new_color.blue += arg * image.GetColor(y, x).blue;

            new_color.red = std::clamp(new_color.red, 0.0f, 1.0f);
            new_color.green = std::clamp(new_color.green, 0.0f, 1.0f);
            new_color.blue = std::clamp(new_color.blue, 0.0f, 1.0f);
            new_image.SetColor(new_color, y, x);
        }
    }
    return new_image;
}

EdgeDetection::EdgeDetection(double threshold) : threshold_(threshold) {
}

Image EdgeDetection::Apply(Image &image) {
    Grayscale grayscale;
    image = grayscale.Apply(image);
    Color new_color;
    Image new_image(image.GetWidth(), image.GetHeight());
    int new_x = 0;
    int new_y = 0;
    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            new_y = std::max(0, y - 1);
            new_x = x;
            new_color.red = -1 * image.GetColor(new_y, new_x).red;

            new_y = std::min(image.GetHeight() - 1, y + 1);
            new_color.red -= image.GetColor(new_y, new_x).red;

            new_x = std::max(0, x - 1);
            new_y = y;
            new_color.red -= image.GetColor(new_y, new_x).red;

            new_x = std::min(image.GetWidth() - 1, x + 1);
            new_color.red -= image.GetColor(new_y, new_x).red;

            new_color.red += 4 * image.GetColor(y, x).red;

            new_color.red = std::clamp(new_color.red, 0.0f, 1.0f);
            if (new_color.red > threshold_) {
                new_color = {1, 1, 1};
            } else {
                new_color = {0, 0, 0};
            }
            new_image.SetColor(new_color, y, x);
        }
    }
    return new_image;
}

GaussianBlur::GaussianBlur(double sigma) : sigma_(sigma) {
}

Image GaussianBlur::Apply(Image &image) {
    const int param1 = 6;
    const double param2 = 0.5;
    const int param3 = 2;
    Image new_image(image.GetWidth(), image.GetHeight());
    int new_x = 0;
    int new_y = 0;
    auto koef1 = std::pow(2 * std::numbers::pi * sigma_ * sigma_, param2);
    auto koef2 = std::pow(std::numbers::e, -1 / (param3 * sigma_ * sigma_));
    double koef3 = 0;
    Color new_color;
    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            new_color = {0, 0, 0};
            for (int k = 0; k < std::ceil(sigma_ * param1); ++k) {
                new_x = k - static_cast<int>(std::ceil(3 * sigma_)) + x;
                koef3 = std::pow(koef2, (new_x - x) * (new_x - x)) / koef1;
                new_x = std::clamp(new_x, 0, image.GetWidth() - 1);
                new_color.red += static_cast<float>(image.GetColor(y, new_x).red * koef3);
                new_color.green += static_cast<float>(image.GetColor(y, new_x).green * koef3);
                new_color.blue += static_cast<float>(image.GetColor(y, new_x).blue * koef3);
            }
            new_image.SetColor(new_color, y, x);
        }
    }

    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            new_color = {0, 0, 0};
            for (int k = 0; k < std::ceil(sigma_ * param1); ++k) {
                new_y = k - static_cast<int>(std::ceil(3 * sigma_)) + y;
                koef3 = std::pow(koef2, (new_y - y) * (new_y - y)) / koef1;
                new_y = std::clamp(new_y, 0, image.GetHeight() - 1);
                new_color.red += static_cast<float>(new_image.GetColor(new_y, x).red * koef3);
                new_color.green += static_cast<float>(new_image.GetColor(new_y, x).green * koef3);
                new_color.blue += static_cast<float>(new_image.GetColor(new_y, x).blue * koef3);
            }
            image.SetColor(new_color, y, x);
        }
    }
    return image;
};

Image MyFilter::Apply(Image &image) {
    const int koef = 8;
    Grayscale grayscale;
    grayscale.Apply(image);
    Image new_image(image.GetWidth(), image.GetHeight());
    Color new_color;
    int new_x = 0;
    int new_y = 0;
    std::vector<std::vector<int>> vec{{1, -1, -1}, {1, 0, -1},   {1, 1, -1},  {0, -1, -1}, {0, 0, koef},
                                      {0, 1, -1},  {-1, -1, -1}, {-1, 0, -1}, {-1, 1, -1}};
    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 1; x < image.GetWidth(); ++x) {
            for (auto &elem : vec) {
                new_x = x + elem[1];
                new_y = y + elem[0];
                new_x = std::max(0, std::min(new_x, image.GetWidth() - 1));
                new_y = std::max(0, std::min(new_y, image.GetHeight() - 1));

                new_color.red += image.GetColor(new_y, new_x).red * static_cast<float>(elem[2]);
                new_color.green += image.GetColor(new_y, new_x).green * static_cast<float>(elem[2]);
                new_color.blue += image.GetColor(new_y, new_x).blue * static_cast<float>(elem[2]);
            }
            new_color.red = std::clamp(new_color.red, 0.0f, 1.0f);
            new_color.green = std::clamp(new_color.green, 0.0f, 1.0f);
            new_color.blue = std::clamp(new_color.blue, 0.0f, 1.0f);
            new_image.SetColor(new_color, y, x);
        }
    }
    return new_image;
}
