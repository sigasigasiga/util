cmake_minimum_required(VERSION 3.28)

project(
    grace
    VERSION 0.1
    LANGUAGES CXX
)

set (CMAKE_CXX_STANDARD 26)

add_subdirectory(lib)
add_subdirectory(test)
