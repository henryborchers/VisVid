cmake_minimum_required(VERSION 3.12)
project(libvisvid VERSION 0.1)

# Set version metadata
set(LIBVISVID_VERSION_MAJOR 0)
set(LIBVISVID_VERSION_MINOR 1)
set(LIBVISVID_VERSION_PATCH 0)
set(LIBVISVID_VERSION "${LIBVISVID_VERSION_MAJOR}.${LIBVISVID_VERSION_MINOR}.${LIBVISVID_VERSION_PATCH}")
string(TIMESTAMP LIBVISVID_VERSION_BUILD "%Y%m%d%H%M%S")

# Set the path for cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

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
if(SDL2_FOUND)
    option(VISVID-DEMO_TESTVISUALS "Test Visualzations" ON)
    option(VISVID-DEMO_NOC_FILE_DIALOG "Include NOC_FILE_DIALOG library" ON)
endif()
include(FetchContent)
FetchContent_Declare(
        libcatch2
        URL https://github.com/catchorg/Catch2/archive/v2.4.1.tar.gz
#        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#        GIT_TAG 6860c8d
)

FetchContent_GetProperties(libcatch2)
if(NOT libcatch2_POPULATED)
    message(STATUS "Fetching Catch2")
    FetchContent_Populate(libcatch2)
        add_subdirectory(${libcatch2_SOURCE_DIR} EXCLUDE_FROM_ALL)
#    add_library(Catch2 INTERFACE)
#    list(APPEND CMAKE_MODULE_PATH ${libcatch2_SOURCE_DIR}/contrib/)
endif()
target_include_directories(Catch2 INTERFACE ${libcatch2_SOURCE_DIR}/single_include)


include(CTest)
enable_testing()

add_subdirectory(src)

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

