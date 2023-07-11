CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

### test
add_executable(cmd_channel_test cmd_channel_test.cpp)
target_link_libraries(cmd_channel_test ${PROJECT_NAME})
add_executable(dir_test dir_test.cpp)
target_link_libraries(dir_test ${PROJECT_NAME})
add_executable(env_test env_test.cpp)
add_executable(poller_test poller_test.cpp)
target_link_libraries(poller_test ${PROJECT_NAME})

### run test
add_test(
  NAME cmd_channel_test
  COMMAND $<TARGET_FILE:cmd_channel_test>
)
add_test(
  NAME dir_test
  COMMAND $<TARGET_FILE:dir_test>
)
add_test(
  NAME env_test
  COMMAND $<TARGET_FILE:env_test>
)
add_test(
  NAME poller_test
  COMMAND $<TARGET_FILE:poller_test>
)
