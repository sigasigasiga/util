# how to build tests on macos using homebrew
# ```
# meson setup --cross-file brew-cross.ini -Dtests=enabled bin
# ```
project('siga-util', 'cpp', default_options : [ 'cpp_std=c++23' ])

siga_util_dep = declare_dependency(
  include_directories: include_directories('src')
)

meson.override_dependency('siga-util', siga_util_dep)

if get_option('tests').enabled()
  test(
    'stored_func_invoker',
    executable(
      'stored_func_invoker',
      'test/stored_func_invoker.cpp',
      dependencies: [siga_util_dep]
    )
  )

  test(
    'expected',
    executable(
      'expected',
      'test/expected.cpp',
      dependencies: [siga_util_dep]
    )
  )

  test(
    'bitset',
    executable(
      'bitset',
      'test/bitset.cpp',
      dependencies: [siga_util_dep]
    )
  )
endif
