CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

### test
add_executable(env_test env_test.cpp)

### run test
add_test(
  NAME env_test
  COMMAND $<TARGET_FILE:env_test>
)
