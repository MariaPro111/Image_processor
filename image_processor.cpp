#include "parser.h"

int main(int argc, char** argv) {
    Parser parser;
    try {
        parser.Parse(argc, argv);

        Image image;
        image.Read(parser.GetInputPath());

        for (auto& filter : parser.GetFilters()) {
            image = filter->Apply(image);
        }

        image.Write(parser.GetOutputPath());
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    return 0;
}
