cmake_minimum_required(VERSION 3.30)

project(
    grace
    VERSION 0.1
    LANGUAGES CXX
)

set (CMAKE_CXX_STANDARD 26)

add_library(grace.fn)
add_library(grace.meta)
add_library(grace.util)

add_subdirectory(fn)
add_subdirectory(meta)
add_subdirectory(util)
