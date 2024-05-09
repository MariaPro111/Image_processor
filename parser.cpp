#include "parser.h"
#include "filter.h"
#include <iostream>
#include <unordered_map>

const char* Parser::GetInputPath() const {
    return input_path_;
}
const std::vector<std::unique_ptr<Filter>>& Parser::GetFilters() const {
    return filters_;
}

const char* Parser::GetOutputPath() const {
    return output_path_;
}

void Parser::Parse(int argc, char** argv) {
    if (argc < 3) {
        throw std::runtime_error("Not enough parameters");
    }

    input_path_ = argv[1];
    output_path_ = argv[2];

    std::vector<std::vector<std::string_view>> filters_vec;
    std::vector<std::string_view> new_filter;
    for (size_t ind = 3; ind < argc; ++ind) {
        if (argv[ind][0] == '-') {
            new_filter.clear();
            new_filter.push_back(argv[ind]);
            filters_vec.push_back(new_filter);
        } else {
            if (filters_vec.empty()) {
                throw std::runtime_error("Incorrect parameters");
            } else {
                filters_vec[filters_vec.size() - 1].push_back(argv[ind]);
            }
        }
    }

    for (const auto& filter : filters_vec) {
        CheckingCorrect(filter);
    }
}

void Parser::CheckingCorrect(const std::vector<std::string_view>& filter) {
    if (filter[0] == "-crop" && filter.size() == 3 && std::all_of(filter[1].begin(), filter[1].end(), isdigit) &&
        std::all_of(filter[2].begin(), filter[2].end(), isdigit)) {
        std::unique_ptr<Filter> crop(new Crop(atoi(filter[1].data()), atoi(filter[2].data())));
        filters_.push_back(std::move(crop));
    } else if (filter[0] == "-gs" && filter.size() == 1) {
        std::unique_ptr<Filter> gr(new Grayscale());
        filters_.push_back(std::move(gr));
    } else if (filter[0] == "-neg" && filter.size() == 1) {
        std::unique_ptr<Filter> neg(new Negative());
        filters_.push_back(std::move(neg));
    } else if (filter[0] == "-sharp" && filter.size() == 1) {
        std::unique_ptr<Filter> sh(new Sharpening());
        filters_.push_back(std::move(sh));
    } else if (filter[0] == "-edge" && filter.size() == 2) {
        try {
            std::stod(filter[1].data());
        } catch (std::invalid_argument& inv) {
            throw std::runtime_error("Incorrect parameters");
        }
        std::unique_ptr<Filter> edge(new EdgeDetection(atof(filter[1].data())));
        filters_.push_back(std::move(edge));
    } else if (filter[0] == "-blur" && filter.size() == 2) {
        try {
            std::stod(filter[1].data());
        } catch (std::invalid_argument& inv) {
            throw std::runtime_error("Incorrect parameters");
        }
        std::unique_ptr<Filter> blur(new GaussianBlur(atof(filter[1].data())));
        filters_.push_back(std::move(blur));
    } else if (filter[0] == "-my" && filter.size() == 1) {
        std::unique_ptr<Filter> my(new MyFilter());
        filters_.push_back(std::move(my));
    } else {
        throw std::runtime_error("Incorrect parameters");
    }
}