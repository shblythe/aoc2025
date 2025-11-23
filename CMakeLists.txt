cmake_minimum_required(VERSION 3.14)

project(aoc2025 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(days)
add_subdirectory(test)
add_subdirectory(utils)
