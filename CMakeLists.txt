CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
PROJECT(common)

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED YES)

SET(CMAKE_C_FLAGS "-fPIC -DPIC -Werror=return-type")
SET(CMAKE_CXX_FLAGS "-fPIC -DPIC -Werror=return-type")
SET(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(GTest REQUIRED)

INCLUDE_DIRECTORIES(.)

add_subdirectory(env)
add_subdirectory(macros)
add_subdirectory(time)
add_subdirectory(dir)
add_subdirectory(string_parser)
