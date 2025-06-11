add_subdirectory(args)

add_library(grace.fn.wrap)

target_sources(
    grace.fn.wrap

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    module.cppm
)

target_link_libraries(
    grace.fn.wrap

    grace.fn.wrap.args
)
