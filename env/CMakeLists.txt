CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

add_executable(env_test env_test.cpp)
target_link_libraries(env_test gtest_main gtest pthread)