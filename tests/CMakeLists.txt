include(FetchContent)
FetchContent_Declare(
        libcatch2
        URL https://github.com/catchorg/Catch2/archive/v2.7.2.tar.gz
)
FetchContent_GetProperties(libcatch2)
if(NOT libcatch2_POPULATED)
    message(STATUS "Fetching Catch2")
    FetchContent_Populate(libcatch2)
    add_library(Catch2 INTERFACE)
    list(APPEND CMAKE_MODULE_PATH ${libcatch2_SOURCE_DIR}/contrib/)
    include(Catch)
    target_include_directories(Catch2 INTERFACE ${libcatch2_SOURCE_DIR}/single_include/catch2)
endif()

add_subdirectory(internal)

add_subdirectory(publicAPI)
