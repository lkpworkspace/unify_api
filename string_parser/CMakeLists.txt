CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

### source
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME_WE)
project(${PROJECT_NAME})
aux_source_directory(. __srcs)

### test
add_subdirectory(test)

### install
file(GLOB header_file "*.h")
install (FILES ${header_file}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    DESTINATION ${UNIFY_API_INC_DIR}/${PROJECT_NAME}
)