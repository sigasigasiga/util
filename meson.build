project('siga-util', 'cpp', default_options : [ 'cpp_std=c++23' ])

siga_util_dep = declare_dependency(
  include_directories: include_directories('src')
)

meson.override_dependency('siga-util', siga_util_dep)

test(
  'stored_func_invoker',
  executable(
    'asdf',
    'test/stored_func_invoker.cpp',
    dependencies: [siga_util_dep]
  )
)
