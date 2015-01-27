cmake_minimum_required(VERSION 2.8.4)
project(fast_join)

set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-std=c++1y)

include_directories(includes)

add_subdirectory(src)
add_subdirectory(bin)
add_subdirectory(tests)
