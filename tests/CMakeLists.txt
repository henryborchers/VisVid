include(ExternalProject)
ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG master
        INSTALL_COMMAND ""
        CMAKE_ARGS -DBUILD_GTEST=ON -DBUILD_GMOCK=OFF -Dgtest_disable_pthreads=ON
        )

ExternalProject_Get_Property(googletest source_dir binary_dir)
include_directories("${source_dir}/googletest/include")
add_library(gtest IMPORTED STATIC GLOBAL)
add_library(gtest_main IMPORTED STATIC GLOBAL)
set_target_properties(gtest PROPERTIES
        IMPORTED_LOCATION ${binary_dir}/googletest/libgtest.a
        )

set_target_properties(gtest_main PROPERTIES
        IMPORTED_LOCATION ${binary_dir}/googletest/libgtest_main.a
        )
add_dependencies(gtest googletest)
add_dependencies(gtest_main googletest)

add_subdirectory(src)