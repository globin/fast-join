set(BIN_SOURCE_FILES "fast-join.cpp")
add_executable(fast-join ${BIN_SOURCE_FILES})
target_link_libraries(fast-join fast_join)
