find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_SOURCE_FILES test.cpp)
add_executable(test ${TEST_SOURCE_FILES})
target_link_libraries(test fast_join ${GTEST_LIBRARIES} pthread)
