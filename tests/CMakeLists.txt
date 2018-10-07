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
if (VISVID_TESTS)

    include(${libcatch2_SOURCE_DIR}/contrib/Catch.cmake)
    add_library(Catch INTERFACE)
    add_library(Catch2::Catch ALIAS Catch)
    target_include_directories(Catch INTERFACE catch2/single_include)

    add_executable(test-visvid test-visBuffer.cpp test-main.cpp test-visFrame.cpp test-visResult.cpp test-visView.cpp test-visVisualization.cpp test-visImageWriter.cpp)

    set_target_properties(test-visvid PROPERTIES
            CXX_STANDARD 11
            )
    target_link_libraries(test-visvid PRIVATE Catch2::Catch Visvid::Visvid)
    catch_discover_tests(test-visvid)
endif (VISVID_TESTS)
