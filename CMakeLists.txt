CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT(unify_api)

### option and var
option(ENABLE_TEST "gen test bin" ON)
if (ENABLE_TEST)
    enable_testing()
endif()

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_THREAD_PREFER_PTHREAD ON)

set(UNIFY_API_INC_DIR ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
set(UNIFY_API_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(UNIFY_API_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

### deps
FIND_PACKAGE(Threads REQUIRED)

### check
include (CheckCXXSourceCompiles)
include (GenerateExportHeader)

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
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

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
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/unify_api_config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/unify_api_version.h @ONLY)
add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/unify_api_version.h)

### sub dir
add_subdirectory(macros)
add_subdirectory(string_parser)
add_subdirectory(cmdline)
add_subdirectory(time)
add_subdirectory(platform)

### export
generate_export_header (${PROJECT_NAME}
  EXPORT_MACRO_NAME UNIFY_API_EXPORT
  EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_export.h)

### install
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${UNIFY_API_LIB_DIR}
    ARCHIVE DESTINATION ${UNIFY_API_LIB_DIR}
    RUNTIME DESTINATION ${UNIFY_API_BIN_DIR}
)
install (FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/unify_api_export.h
  ${CMAKE_CURRENT_SOURCE_DIR}/unify_api_version.h
  ${CMAKE_CURRENT_SOURCE_DIR}/unify_api_platform.h
  DESTINATION ${UNIFY_API_INC_DIR})
install(CODE "
  file(
    WRITE ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}_version.txt
    \"${GIT_BRANCH}\n${GIT_COMMIT_ID}\n${GIT_COMMIT_DATE}\n\")
")
