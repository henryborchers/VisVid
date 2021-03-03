cmake_minimum_required(VERSION 3.12)

project(libvisvid
        VERSION 0.1.0
        DESCRIPTION "Turns video data into a visual map"
        )
option(libvisvid_build_examples "Build example files")
option(${PROJECT_NAME}_TESTS "Build test files" OFF)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Set version metadata
string(TIMESTAMP LIBVISVID_VERSION_BUILD "%Y%m%d%H%M%S")


find_package(FFMPEG
    COMPONENTS
        avformat
        avcodec
        avutil
        swscale
    )

find_package(SDL2)
find_package(Doxygen)
find_package(Python3 COMPONENTS Interpreter Development)

#include(CTest)


add_subdirectory(src)

if(libvisvid_build_examples)
    add_subdirectory(examples)
endif(libvisvid_build_examples)


# build tests
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Build Documentation
include(cmake/buildDocumentation.cmake)

# Enable packaging
include(cmake/CPackConfig.cmake)
include(CPack)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/VisVidConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/VisVidConfig.cmake
    INSTALL_DESTINATION share/cmake/${PROJECT_NAME}

)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/VisVidConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/VisVidConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/VisVidConfigVersion.cmake
    DESTINATION
        share/cmake/${PROJECT_NAME}
)