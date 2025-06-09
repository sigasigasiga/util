add_library(grace.utility)

target_sources(
    grace.utility

    PUBLIC

    FILE_SET
    CXX_MODULES FILES

    copy.cppm
    cut_rvalue_ref.cppm
    decay_copy.cppm
    default_assign.cppm
    get_reference.cppm
    ignore.cppm
    implicit_cast.cppm
    move_only.cppm
    private_base_cast.cppm
    scoped.cppm

    module.cppm
)
