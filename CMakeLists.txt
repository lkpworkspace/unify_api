CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT(unify_api)

### option
option(ENABLE_TEST "gen test bin" ON)
if (ENABLE_TEST)
    enable_testing()
endif()

### install path
set(UNIFY_API_INC_DIR ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
set(UNIFY_API_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(UNIFY_API_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

### deps
FIND_PACKAGE(Threads REQUIRED)

### check
include (CheckCXXSourceCompiles)

check_cxx_source_compiles ("
#include <cstdlib>
#include <ctime>
int main() {
  time_t timep;
  struct tm result;
  localtime_r(&timep, &result);
  return EXIT_SUCCESS;
}
" HAVE_LOCALTIME_R)

### include
INCLUDE_DIRECTORIES(.)

### sub dir
add_subdirectory(env)
add_subdirectory(macros)
add_subdirectory(time)
add_subdirectory(dir)
add_subdirectory(string_parser)

### version
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git rev-parse HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_ID
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git show -s --format=%ci HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_DATE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

### unify_api lib
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp @ONLY)
add_library(${PROJECT_NAME} SHARED version.cpp)
target_link_libraries(${PROJECT_NAME}
    unify_api_dir
    unify_api_time
)

### install
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${UNIFY_API_LIB_DIR}
    ARCHIVE DESTINATION ${UNIFY_API_LIB_DIR}
    RUNTIME DESTINATION ${UNIFY_API_BIN_DIR}
)
install(CODE "
  file(
    WRITE ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}_version.txt
    \"${GIT_BRANCH}\n${GIT_COMMIT_ID}\n${GIT_COMMIT_DATE}\n\")
")
