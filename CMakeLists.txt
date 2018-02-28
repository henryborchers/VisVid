cmake_minimum_required(VERSION 3.6)
project(libvisvid VERSION 0.1)

# Set version metadata
set(LIBVISVID_VERSION_MAJOR 0)
set(LIBVISVID_VERSION_MINOR 1)
set(LIBVISVID_VERSION_PATCH 0)
set(LIBVISVID_VERSION "${LIBVISVID_VERSION_MAJOR}.${LIBVISVID_VERSION_MINOR}.${LIBVISVID_VERSION_PATCH}")
string(TIMESTAMP LIBVISVID_VERSION_BUILD "%Y%m%d%H%M%S")

# Set the path for cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Set default compiling flags
find_package(FFMPEG REQUIRED)
find_package(SDL2)
find_package(Doxygen)


# Set Options
if (DOXYGEN_FOUND)
    option(VISVID_BUILDDOCS "Build Doxygen documentation" OFF)
endif ()

option(VISVID_TESTS "Build Tests" ON)
option(VISVID_MEMDEBUG "Enable memory allocation messages" OFF)
option(VISVID-DEMO_TESTVISUALS "Test Visualzations" ON)
option(VISVID-DEMO_NOC_FILE_DIALOG "Include NOC_FILE_DIALOG library" ON)

enable_testing()
add_subdirectory(lib)

#build application
if (VISVID-DEMO_TESTVISUALS)
    add_subdirectory(src)
endif ()
# build tests
if (VISVID_TESTS)
    add_subdirectory(tests)
endif (VISVID_TESTS)

# Build Documentation
include(cmake/buildDocumentation.cmake)

# Build Swig Bindings
include(swig/swig.cmake)


# Enable packaging
include(cmake/CPackConfig.cmake)
include(CPack)

