# TODO: add `lift.hpp`

target_sources(
    grace.fn

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    casts.cppm
    construct.cppm
    copy.cppm
    cut_rvalue_ref.cppm
    decay_copy.cppm
    for_each_in_parameter_pack.cppm
    free.cppm
    get.cppm
    get_reference.cppm
    indirect.cppm
    module.cppm
    subscript.cppm
    throw_exception.cppm
    tuple_fold_invoke.cppm
    tuple_sequence_invoke.cppm
)

target_link_libraries(
    grace.fn

    grace.meta
    grace.util
)
