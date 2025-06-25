add_library(grace.algorithm)

target_sources(
    grace.algorithm

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    clamp_to.cppm

    module.cppm
)
