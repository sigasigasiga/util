# testing binary
add_executable(stored_func_invoker stored_func_invoker.cpp)
target_link_libraries(stored_func_invoker PRIVATE grace.fn.bind grace.utility)

add_executable(fold_invoke fold_invoke.cpp)
target_link_libraries(fold_invoke PRIVATE grace.fn.invoke)

# enable testing functionality
enable_testing()

# define tests
add_test(
  NAME stored_func_invoker
  COMMAND $<TARGET_FILE:stored_func_invoker>
)

add_test(
  NAME fold_invoke
  COMMAND $<TARGET_FILE:fold_invoke>
)
