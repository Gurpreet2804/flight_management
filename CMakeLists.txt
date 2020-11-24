#
# @file
# @copyright Copyright (C) 2020, KPIT Technologies
#
cmake_minimum_required(VERSION 3.10)
project(flight_management)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_subdirectory(lib/googletest)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_library(flight_management STATIC ${SOURCES})
file(GLOB SOURCES "test/*.cpp")
add_executable(flight_management_test ${SOURCES})
target_link_libraries(flight_management_test flight_management gtest)