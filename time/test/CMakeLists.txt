CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

### test
add_executable(time_test time_test.cpp)
target_link_libraries(time_test ${PROJECT_NAME})

### add test
add_test(
  NAME time_test
  COMMAND $<TARGET_FILE:time_test>
)
