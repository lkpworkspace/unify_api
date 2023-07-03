CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

### test
add_executable(string_parser_test string_parser_test.cpp)

### run test
add_test(
  NAME string_parser_test
  COMMAND $<TARGET_FILE:string_parser_test>
)
