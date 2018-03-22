set(CTEST_SOURCE_DIRECTORY .)
set(CTEST_BINARY_DIRECTORY  ./build)

set(ENV{CXXFLAGS} "--coverage")
set(CTEST_CMAKE_GENERATOR "Unix Makefiles")

set(CTEST_COVERAGE_COMMAND "gcov")
set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrind")

ctest_start("Continuous")
ctest_update()
ctest_configure()
ctest_build()
ctest_test()
ctest_coverage()
ctest_memcheck()
#ctest_submit()