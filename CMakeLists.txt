cmake_minimum_required(VERSION 3.6)
project(Visvid)

# Set version metadata
set(LIBRARY_VERSION_MAJOR 0)
set(LIBRARY_VERSION_MINOR 1)
set(LIBRARY_VERSION_PATCH 0)
string(TIMESTAMP LIBRARY_VERSION_BUILD "%Y%m%d%H%M%S")

# Set default compiling flags
set(CMAKE_C_STANDARD 99)


# Set Options
set(VISVID_BUILDDOCS ON CACHE BOOL "Build Doxygen documentation")
set(VISVID_TESTS ON CACHE BOOL "Build Tests")

enable_testing()
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
