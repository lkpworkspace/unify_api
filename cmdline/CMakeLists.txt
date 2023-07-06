CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

### source
get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME_WE)

### test
add_subdirectory(test)

### install
file(GLOB header_file "*.h")
install (FILES ${header_file}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    DESTINATION ${UNIFY_API_INC_DIR}/${DIR_NAME}
)
