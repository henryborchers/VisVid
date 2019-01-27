add_executable(test-visvid EXCLUDE_FROM_ALL
        ../test-main.cpp
        test-visBuffer.cpp
        test-visFrame.cpp
        test-visResult.cpp
        test-visView.cpp
        )

set_target_properties(test-visvid PROPERTIES
        CXX_STANDARD 11
        )
target_link_libraries(test-visvid PRIVATE Catch2 Visvid::Visvid)
catch_discover_tests(test-visvid TEST_PREFIX ${PROJECT_NAME}::test-visvid::)