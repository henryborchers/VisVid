cmake_minimum_required(VERSION 3.6)
project(Visvid)

set(LIBRARY_VERSION_MAJOR 0)
set(LIBRARY_VERSION_MINOR 1)
set(LIBRARY_VERSION_PATCH 0)
string(TIMESTAMP LIBRARY_VERSION_BUILD "%Y%m%d%H%M%S")
enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set Options
set(VISVID_BUILDDOCS ON CACHE BOOL "Build Doxygen documentation")
set(VISVID_TESTS ON CACHE BOOL "Build Tests")

add_subdirectory(lib)
add_subdirectory(src)

# build tests
if(VISVID_TESTS)
    add_subdirectory(tests)
endif(VISVID_TESTS)

# Build Documentation
include(cmake/buildDocumentation.cmake)

# Build Swig Bindings
include(swig/swig.cmake)
