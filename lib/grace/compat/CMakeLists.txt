add_library(grace.compat)

target_sources(
    grace.compat

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    bind_back.cppm

    module.cppm
)
