add_library(grace.fn.bind)

target_sources(
    grace.fn.bind

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    lift_function.cppm
    return_value.cppm

    module.cppm
)
