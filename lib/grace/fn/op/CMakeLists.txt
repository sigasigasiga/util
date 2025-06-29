add_library(grace.fn.op)

target_sources(
    grace.fn.op

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    cast.cppm
    construct.cppm
    copy.cppm
    cut_rvalue_ref.cppm
    decay_copy.cppm
    free.cppm
    get.cppm
    indirect.cppm
    objectify.cppm
    ref.cppm
    subscript.cppm
    throw_exception.cppm

    module.cppm
)

target_link_libraries(
    grace.fn.op

    grace.fn.invoke
    grace.tuple
    grace.utility
)
