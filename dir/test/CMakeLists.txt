CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

### test
add_executable(dir_test dir_test.cpp)
target_link_libraries(dir_test
    unify_api_dir
)

### run test
add_test(
  NAME dir_test
  COMMAND $<TARGET_FILE:dir_test>
)
