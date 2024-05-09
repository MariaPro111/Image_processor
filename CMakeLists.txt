add_executable(image_processor image_processor.cpp parser.cpp filter.cpp image.cpp)

    add_catch(test_image_processor test.cpp parser.cpp filter.cpp image.cpp)
