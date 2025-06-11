add_library(grace.fn.wrap.args)

target_sources(
    grace.fn.wrap.args

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    ignore.cppm
    unwrap_reference.cppm

    module.cppm
)

target_link_libraries(
    grace.fn.wrap.args

    grace.type_traits
    grace.utility
)
