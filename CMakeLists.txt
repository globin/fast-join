cmake_minimum_required(VERSION 2.8.4)
project(fast_join)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(includes)

add_subdirectory(src)
add_subdirectory(bin)
add_subdirectory(tests)
