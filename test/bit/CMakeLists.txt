# testing binary
add_executable(make_bitset make_bitset.cpp)
target_link_libraries(make_bitset PRIVATE grace.bit)

# enable testing functionality
enable_testing()

# define tests
add_test(
  NAME make_bitset
  COMMAND $<TARGET_FILE:make_bitset>
)
