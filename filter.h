
#pragma once
#include "image.h"
#ifndef CPP_HSE_FILTER_H
#define CPP_HSE_FILTER_H

#endif  // CPP_HSE_FILTER_H

class Filter {
public:
    virtual Image Apply(Image &image) = 0;
    virtual ~Filter() = default;
};

class Crop : public Filter {
public:
    Crop(int width, int height);
    Image Apply(Image &image) override;
    ~Crop() override = default;

private:
    int width_ = 0;
    int height_ = 0;
};

class Grayscale : public Filter {
public:
    Grayscale() = default;
    Image Apply(Image &image) override;
    ~Grayscale() override = default;
};

class Negative : public Filter {
public:
    Negative() = default;
    Image Apply(Image &image) override;
    ~Negative() override = default;
};

class Sharpening : public Filter {
public:
    Sharpening() = default;
    Image Apply(Image &image) override;
    ~Sharpening() override = default;
};

class EdgeDetection : public Filter {
public:
    explicit EdgeDetection(double threshold);
    Image Apply(Image &image) override;
    ~EdgeDetection() override = default;

private:
    double threshold_ = 0;
};

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(double sigma);
    Image Apply(Image &image) override;
    ~GaussianBlur() override = default;

private:
    double sigma_ = 0;
};

class MyFilter : public Filter {
public:
    MyFilter() = default;
    Image Apply(Image &image) override;
    ~MyFilter() override = default;
};