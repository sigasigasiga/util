add_library(grace.concepts)

target_sources(
    grace.concepts

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    conceptify.cppm

    module.cppm
)
