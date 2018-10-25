#add_subdirectory(catch2)
#include(ExternalProject)
#ExternalProject_Add(googletest
#        GIT_REPOSITORY https://github.com/google/googletest
#        GIT_TAG master
#        INSTALL_COMMAND ""
#        CMAKE_ARGS -DBUILD_GTEST=ON -DBUILD_GMOCK=OFF -Dgtest_disable_pthreads=ON
#        )
#
#ExternalProject_Get_Property(googletest source_dir binary_dir)
#include_directories("${source_dir}/googletest/include")
#
#add_library(gtest IMPORTED STATIC GLOBAL)
#add_library(gtest_main IMPORTED STATIC GLOBAL)
#set_target_properties(gtest PROPERTIES
#        IMPORTED_LOCATION ${binary_dir}/googletest/libgtest.a
#        )
#
#set_target_properties(gtest_main PROPERTIES
#        IMPORTED_LOCATION ${binary_dir}/googletest/libgtest_main.a
#        )
#add_dependencies(gtest googletest)
#add_dependencies(gtest_main googletest)
#
#add_subdirectory(src)

##############################
# Catch2 tests
##############################
#if (VISVID_TESTS)
include(FetchContent)
FetchContent_Declare(
        libcatch2
        #        URL https://github.com/catchorg/Catch2/archive/v2.4.1.tar.gz
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 6860c8d
)

FetchContent_GetProperties(libcatch2)

if(NOT libcatch2_POPULATED)
    message(STATUS "Fetching Catch2")
    FetchContent_Populate(libcatch2)
#    add_subdirectory(${libcatch2_SOURCE_DIR}
#            EXCLUDE_FROM_ALL
#            )
    add_library(Catch2 INTERFACE)
    list(APPEND CMAKE_MODULE_PATH ${libcatch2_SOURCE_DIR}/contrib/)
#else()
    #    message(STATUS "Catch2 exists")
endif()
target_include_directories(Catch2 INTERFACE ${libcatch2_SOURCE_DIR}/single_include)
include(${libcatch2_SOURCE_DIR}/contrib/ParseAndAddCatchTests.cmake)
#if(libcatch2_POPULATED)
add_library(Catch INTERFACE)
add_library(Catch2::Catch ALIAS Catch)
#endif(libcatch2_POPULATED)

target_include_directories(Catch INTERFACE ${libcatch2_SOURCE_DIR}/single_include)

add_executable(test-visvid EXCLUDE_FROM_ALL test-visBuffer.cpp test-main.cpp test-visFrame.cpp test-visResult.cpp test-visView.cpp test-visVisualization.cpp test-visImageWriter.cpp)

set_target_properties(test-visvid PROPERTIES
        CXX_STANDARD 11
        )
target_link_libraries(test-visvid PRIVATE Catch2::Catch Visvid::Visvid)
ParseAndAddCatchTests(test-visvid)
#endif (VISVID_TESTS)
