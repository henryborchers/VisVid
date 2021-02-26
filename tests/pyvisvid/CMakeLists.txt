add_executable(test_pyvisvid ../test-main.cpp test_pyvisvid.cpp)
set_target_properties(test_pyvisvid PROPERTIES
        CXX_STANDARD 11
        )
target_link_libraries(test_pyvisvid PRIVATE pyvisvid_module Catch2)


catch_discover_tests(test_pyvisvid TEST_PREFIX ${PROJECT_NAME}::test_pyvisvid::)
