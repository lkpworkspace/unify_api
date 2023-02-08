CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

add_executable(string_parser_test string_parser_test.cpp)
target_link_libraries(string_parser_test gtest_main gtest pthread)
