CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

add_executable(dir_test dir_test.cpp dir.cpp)
target_link_libraries(dir_test gtest_main gtest pthread)