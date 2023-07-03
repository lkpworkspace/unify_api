CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

### source
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME_WE)
project(${PROJECT_NAME})
aux_source_directory(. __srcs)

### lib
add_library(unify_api_dir OBJECT ${__srcs})

### test
add_subdirectory(test)
