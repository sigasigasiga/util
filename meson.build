# how to build tests on macos using homebrew
# ```
# meson setup --cross-file brew-cross.ini -Dtests=enabled bin
# ```
project('siga-util', 'cpp', default_options : [ 'cpp_std=c++26' ])

siga_util_dep = declare_dependency(
  include_directories: include_directories('include')
)

meson.override_dependency('siga-util', siga_util_dep)

if get_option('tests').enabled()
  test(
    'make_bitset',
    executable(
      'make_bitset',
      'test/bitset.cpp',
      dependencies: [siga_util_dep]
    )
  )

  test(
    'forward_ref',
    executable(
      'forward_ref',
      'test/forward_ref.cpp',
      dependencies: [siga_util_dep]
    )
  )

  test(
    'read_only_value',
    executable(
      'read_only_value',
      'test/read_only_value.cpp',
      dependencies: [siga_util_dep]
    )
  )

  test(
    'stored_func_invoker',
    executable(
      'stored_func_invoker',
      'test/stored_func_invoker.cpp',
      dependencies: [siga_util_dep]
    )
  )

  test(
    'include_all.cpp',
    executable(
      'include_all',
      'test/include_all.cpp',
      dependencies: [siga_util_dep]
    )
  )

  test(
    'tuple_fold_invoke.cpp',
    executable(
      'tuple_fold_invoke',
      'test/tuple_fold_invoke.cpp',
      dependencies: [siga_util_dep]
    )
  )

  test(
    'consign_view.cpp',
    executable(
      'consign_view',
      'test/consign_view.cpp',
      dependencies: [siga_util_dep]
    )
  )

  test(
    'to_address.cpp',
    executable(
      'to_address',
      'test/to_address.cpp',
      dependencies: [siga_util_dep]
    )
  )
endif
