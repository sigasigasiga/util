add_library(grace.memory)

target_sources(
    grace.memory

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    get_deleter.cppm
    shared_from_this_base.cppm
    to_address.cppm
    to_address_arr.cppm

    module.cppm
)

target_link_libraries(
    grace.memory

    grace.meta
    grace.type_traits
)
