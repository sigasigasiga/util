add_library(grace.meta)

target_sources(
    grace.meta

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    overload_priority.cppm
    string_literal.cppm
    tag.cppm
    type_list.cppm

    module.cppm
)
