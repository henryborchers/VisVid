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
set(VISVID_PACKAGE ON CACHE BOOL "Enable CPack")

set(APPLICATION_NOC_FILE_DIALOG ON CACHE BOOL "Include NOC_FILE_DIALOG library")
set(APPLICATION_TESTVISUALS ON CACHE BOOL "Test Visualzations")

enable_testing()
add_subdirectory(lib)

#build application
if(APPLICATION_TESTVISUALS)
    add_subdirectory(src)
endif()
# build tests
if(VISVID_TESTS)
    add_subdirectory(tests)
endif(VISVID_TESTS)

# Build Documentation
include(cmake/buildDocumentation.cmake)

# Build Swig Bindings
include(swig/swig.cmake)

if (VISVID_PACKAGE)
    set(CPACK_PACKAGE_NAME "Visvid")
    set(CPACK_PACKAGE_VENDOR "Henry Borchers")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/license.txt")
    set(CPACK_COMPONENT_LIBVISVID_REQUIRED TRUE)

    if(APPLICATION_TESTVISUALS)
        set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Create Visual")
        set(CPACK_PACKAGE_EXECUTABLES CreateVisual "Test Visualzations")
    endif(APPLICATION_TESTVISUALS)


    # Enable packaging
    include(cpack)

endif ()
