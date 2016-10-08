include(ExternalProject)
set(PThreadLib -pthread)
ExternalProject_Add(googletest
        URL https://github.com/google/googletest/archive/release-1.7.0.zip
        URL_HASH SHA1=f89bc9f55477df2fde082481e2d709bfafdb057b
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/googleTest
        INSTALL_COMMAND ""
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest"
        )

ExternalProject_Get_Property(googletest source_dir binary_dir)

include_directories("${source_dir}/include")
add_library(gtest      IMPORTED STATIC GLOBAL)
add_library(gtest_main IMPORTED STATIC GLOBAL)
set_target_properties(gtest PROPERTIES
        IMPORTED_LOCATION ${binary_dir}/libgtest.a
        )

set_target_properties(gtest_main PROPERTIES
        IMPORTED_LOCATION ${binary_dir}/libgtest_main.a
        )
add_dependencies(gtest      googletest)
add_dependencies(gtest_main googletest)

add_subdirectory(src)