include(FetchContent)
FetchContent_Declare(
        libcatch2
        URL https://github.com/catchorg/Catch2/archive/v2.5.0.tar.gz
        #        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        #        GIT_TAG 6860c8d
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

add_executable(test-visvid EXCLUDE_FROM_ALL
        test-visBuffer.cpp
        test-main.cpp
        test-visFrame.cpp
        test-visResult.cpp
        test-visView.cpp
        test-visVisualization.cpp
        test-visImageWriter.cpp
        )

set_target_properties(test-visvid PROPERTIES
        CXX_STANDARD 11
        )
target_link_libraries(test-visvid PRIVATE Catch2 Visvid::Visvid)
#ParseAndAddCatchTests(test-visvid)
catch_discover_tests(test-visvid TEST_PREFIX ${PROJECT_NAME}::test-visvid::)
#endif (VISVID_TESTS)
#add_dependencies(test test-visvid)