target_sources(
    grace.meta

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    concepts.cppm
    copy_cvref.cppm
    index_sequence_for_tuple.cppm
    string_literal.cppm
    tag.cppm
    tuple.cppm
    type_list.cppm

    module.cppm
)
