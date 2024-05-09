
#pragma once
#ifndef CPP_HSE_PARSER_H
#define CPP_HSE_PARSER_H

#endif  // CPP_HSE_PARSER_H
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "filter.h"

class Parser {
public:
    Parser() = default;
    void Parse(int argc, char** argv);
    const char* GetInputPath() const;
    const char* GetOutputPath() const;
    const std::vector<std::unique_ptr<Filter>>& GetFilters() const;
    void CheckingCorrect(const std::vector<std::string_view>& filter);

private:
    const char* input_path_;
    const char* output_path_;
    std::vector<std::unique_ptr<Filter>> filters_;
};