cmake_minimum_required(VERSION 3.12)
project(libvisvid VERSION 0.1)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Set version metadata
set(LIBVISVID_VERSION_MAJOR 0)
set(LIBVISVID_VERSION_MINOR 1)
set(LIBVISVID_VERSION_PATCH 0)
set(LIBVISVID_VERSION "${LIBVISVID_VERSION_MAJOR}.${LIBVISVID_VERSION_MINOR}.${LIBVISVID_VERSION_PATCH}")
string(TIMESTAMP LIBVISVID_VERSION_BUILD "%Y%m%d%H%M%S")

# Set the path for cmake modules
#list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#
# Set default compiling flags
find_package(FFMPEG
        COMPONENTS
            avformat
            avcodec
            avutil
            swscale
        )
find_package(SDL2)
find_package(Doxygen)


#option(VISVID_TESTS "Build Tests" ON)
option(VISVID_MEMDEBUG "Enable memory allocation messages" OFF)

#if(SDL2_FOUND)
#    option(VISVID-DEMO_NOC_FILE_DIALOG "Include NOC_FILE_DIALOG library" ON)
#endif()



include(CTest)
enable_testing()

add_subdirectory(src)

# build tests
add_subdirectory(tests)

# Build Documentation
include(cmake/buildDocumentation.cmake)

# Enable packaging
include(cmake/CPackConfig.cmake)
include(CPack)

