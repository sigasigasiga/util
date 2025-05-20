target_sources(
    grace.util

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    bitset.cppm
    cctype.cppm
    exception_storage.cppm
    first_flag.cppm
    flag_set.cppm
    for_each_in_parameter_pack.cppm
    for_each_in_tuple.cppm
    get_deleter.cppm

    utility.cppm

    module.cppm
)

target_link_libraries(
    grace.util

    grace.fn
    grace.meta
)
