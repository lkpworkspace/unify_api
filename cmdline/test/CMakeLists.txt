CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

### test
add_executable(cmdline_test test.cpp)
add_executable(cmdline2_test test2.cpp)

### run test
add_test(
  NAME cmdline_test
  COMMAND $<TARGET_FILE:cmdline_test>
)
add_test(
  NAME cmdline2_test
  COMMAND $<TARGET_FILE:cmdline2_test>
)