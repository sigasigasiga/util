add_library(grace.bit)

target_sources(
    grace.bit

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    flag_set.cppm
    make_bitset.cppm

    module.cppm
)
