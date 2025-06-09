add_library(grace.tuple)

target_sources(
    grace.tuple

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    index_sequence_for_tuple.cppm

    module.cppm
)
