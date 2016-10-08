cmake_minimum_required(VERSION 3.6)
project(Visvid)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)