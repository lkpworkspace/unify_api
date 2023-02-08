CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

add_executable(time_test 
    time_test.cpp
    duration.cpp
    rate.cpp
    time.cpp
)
target_link_libraries(time_test gtest_main gtest pthread)
