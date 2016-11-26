cmake_minimum_required(VERSION 3.6)
project(libvisvid)

# Set version metadata
set(LIBVISVID_VERSION_MAJOR 0)
set(LIBVISVID_VERSION_MINOR 1)
set(LIBVISVID_VERSION_PATCH 0)
set(LIBVISVID_VERSION "${LIBVISVID_VERSION_MAJOR}.${LIBVISVID_VERSION_MINOR}.${LIBVISVID_VERSION_PATCH}")
string(TIMESTAMP LIBVISVID_VERSION_BUILD "%Y%m%d%H%M%S")

# Set the path for cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Set default compiling flags
set(CMAKE_C_STANDARD 99)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

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