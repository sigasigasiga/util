# testing binary
add_executable(tuple_like tuple_like.cpp)
target_link_libraries(tuple_like PRIVATE grace.tuple)

# enable testing functionality
enable_testing()

# define tests
add_test(
  NAME tuple_like
  COMMAND $<TARGET_FILE:tuple_like>
)
