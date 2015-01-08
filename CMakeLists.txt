cmake_minimum_required(VERSION 2.8.4)
project(fast_join)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(src/includes)
set(SOURCE_FILES src/fast_join.cpp
                 src/fuzzy_signature.cpp
                 src/fuzzy_similarity.cpp)
add_library(fast_join ${SOURCE_FILES})
target_link_libraries(fast_join simmetrics)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_SOURCE_FILES tests/test.cpp)
add_executable(test ${TEST_SOURCE_FILES})
target_link_libraries(test fast_join ${GTEST_LIBRARIES} pthread)
