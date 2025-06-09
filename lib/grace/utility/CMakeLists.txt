add_library(grace.utility)

target_sources(
    grace.utility

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    cut_rvalue_ref.cppm
    ignore.cppm

    module.cppm
)
