
#pragma once
#ifndef CPP_HSE_IMAGE_H
#define CPP_HSE_IMAGE_H

#endif  // CPP_HSE_IMAGE_H

#include <vector>

struct Color {
    float red = 0;
    float green = 0;
    float blue = 0;
};

class Image {
public:
    Image();
    Image(int width, int height);
    Color GetColor(int y, int x) const;
    void SetColor(Color& color, int y, int x);
    int GetWidth() const;
    int GetHeight() const;

    void Read(const char* path);
    void Write(const char* path) const;

private:
    int m_width_;
    int m_height_;
    std::vector<std::vector<Color>> m_colors_;
};
