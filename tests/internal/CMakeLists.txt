add_executable(test-visvid-internal
        ../test-main.cpp
        test-internal-visbuffer.cpp
        test-internal-visFrame.cpp
        test-internal-visImageWriter.cpp
        test-internal-visResult.cpp
        test-internal-visView.cpp
        test-internal-visVisualization.cpp
        )

set_target_properties(test-visvid-internal PROPERTIES
        CXX_STANDARD 11
        )
if(CMAKE_CXX_CLANG_TIDY)
    set_target_properties(test-visvid-internal PROPERTIES
        CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY};-checks=-clang-diagnostic-deprecated-*"
    )
endif(CMAKE_CXX_CLANG_TIDY)

target_link_libraries(test-visvid-internal PRIVATE Catch2 Visvid::Visvid)
catch_discover_tests(test-visvid-internal TEST_PREFIX ${PROJECT_NAME}::test-visvid::internal::)