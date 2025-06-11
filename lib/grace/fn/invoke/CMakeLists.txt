add_library(grace.fn.invoke)

target_sources(
    grace.fn.invoke

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    fold.cppm
    reverse_arguments.cppm
    sequence.cppm

    module.cppm
)

target_link_libraries(
    grace.fn.invoke

    grace.tuple
)
