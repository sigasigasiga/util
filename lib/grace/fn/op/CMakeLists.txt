add_library(grace.fn.op)

target_sources(
    grace.fn.op

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    cast.cppm

    module.cppm
)
