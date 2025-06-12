# testing binary
add_executable(prepend_deleter prepend_deleter.cpp)
target_link_libraries(prepend_deleter PRIVATE grace.memory)

# enable testing functionality
enable_testing()

# define tests
add_test(
  NAME prepend_deleter
  COMMAND $<TARGET_FILE:prepend_deleter>
)
