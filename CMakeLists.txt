cmake_minimum_required(VERSION 3.6)
SET(LIBRARY_VERSION "0.0.1")
project(Visvid)
enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)

# Build Documentation
include(cmake/buildDocumentation.cmake)

include(swig/swig.cmake)