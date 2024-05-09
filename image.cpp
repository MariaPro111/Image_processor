#include "image.h"
#include <fstream>
#include <iostream>

// NOLINTBEGIN
Color Image::GetColor(int y, int x) const {
    return m_colors_[y][x];
}

Image::Image(){};

Image::Image(int width, int height)
    : m_width_(width), m_height_(height), m_colors_(std::vector<std::vector<Color>>(height)) {
    for (auto& elem : m_colors_) {
        elem.resize(m_width_);
    }
};

int Image::GetHeight() const {
    return m_height_;
}

int Image::GetWidth() const {
    return m_width_;
}

void Image::SetColor(Color& color, int y, int x) {
    m_colors_[y][x].red = color.red;
    m_colors_[y][x].green = color.green;
    m_colors_[y][x].blue = color.blue;
}

void Image::Read(const char* path) {
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);

    if (!f.is_open()) {
        throw std::runtime_error("File could not be opened");
    }
    const int file_header_size = 14;
    const int information_header_size = 40;

    unsigned char file_header[file_header_size];
    f.read(reinterpret_cast<char*>(file_header), file_header_size);

    unsigned char information_header[information_header_size];
    f.read(reinterpret_cast<char*>(information_header), information_header_size);

    if (file_header[0] != 'B' || file_header[1] != 'M') {
        f.close();
        throw std::runtime_error("Invalid file type");
    }

    m_width_ = information_header[4] + (information_header[5] << 8) + (information_header[6] << 16) +
               (information_header[7] << 24);
    m_height_ = information_header[8] + (information_header[9] << 8) + (information_header[10] << 16) +
                (information_header[11] << 24);

    m_colors_.resize(m_height_);
    for (auto& elem : m_colors_) {
        elem.resize(m_width_);
    }

    const int padding_amount = ((4 - (m_width_ * 3) % 4) % 4);

    const float div = 255.0f;
    for (int y = 0; y < m_height_; y++) {
        for (int x = 0; x < m_width_; x++) {
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);
            m_colors_[y][x].red = static_cast<float>(color[2]) / div;
            m_colors_[y][x].green = static_cast<float>(color[1]) / div;
            m_colors_[y][x].blue = static_cast<float>(color[0]) / div;
        }
        f.ignore(padding_amount);
    }
    f.close();
}

void Image::Write(const char* path) const {

    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if (!f.is_open()) {
        throw std::runtime_error("File could not be opened");
    }

    unsigned char bmp_pad[3] = {0, 0, 0};
    const int padding_amount = ((4 - (m_width_ * 3) % 4) % 4);

    const int file_header_size = 14;
    const int information_header_size = 40;
    const int file_size =
        file_header_size + information_header_size + m_width_ * m_height_ * 3 + padding_amount * m_width_;

    unsigned char file_header[file_header_size];

    file_header[0] = 'B';
    file_header[1] = 'M';

    file_header[2] = file_size;
    file_header[3] = file_size >> 8;
    file_header[4] = file_size >> 16;
    file_header[5] = file_size >> 24;

    for (int i = 6; i < file_header_size; ++i) {
        if (i == 10) {
            file_header[i] = file_header_size + information_header_size;
        } else {
            file_header[i] = 0;
        }
    }

    unsigned char information_header[information_header_size];

    information_header[0] = information_header_size;
    information_header[1] = 0;
    information_header[2] = 0;
    information_header[3] = 0;

    information_header[4] = m_width_;
    information_header[5] = m_width_ >> 8;
    information_header[6] = m_width_ >> 16;
    information_header[7] = m_width_ >> 24;

    information_header[8] = m_height_;
    information_header[9] = m_height_ >> 8;
    information_header[10] = m_height_ >> 16;
    information_header[11] = m_height_ >> 24;

    information_header[12] = 1;
    information_header[13] = 0;
    information_header[14] = 24;
    information_header[15] = 0;

    for (int i = 16; i < information_header_size; ++i) {
        information_header[i] = 0;
    }

    f.write(reinterpret_cast<char*>(file_header), file_header_size);
    f.write(reinterpret_cast<char*>(information_header), information_header_size);

    for (int y = 0; y < m_height_; y++) {
        for (int x = 0; x < m_width_; x++) {
            unsigned char r = static_cast<unsigned char>(GetColor(y, x).red * 255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(y, x).green * 255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(y, x).blue * 255.0f);

            unsigned char color[] = {b, g, r};

            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmp_pad), padding_amount);
    }
    f.close();
}  // NOLINTEND