# testing binary
add_executable(stored_func_invoker stored_func_invoker.cpp)
target_link_libraries(stored_func_invoker PRIVATE grace.fn.bind grace.utility)

# enable testing functionality
enable_testing()

# define tests
add_test(
  NAME stored_func_invoker
  COMMAND $<TARGET_FILE:stored_func_invoker>
)
