include(GoogleTest)
INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(TEST_SRC
#        TESTvisFrame.cpp
#        TESTvisBuffer.cpp
#        TESTvisResult.cpp
#        TESTvisView.cpp
        TESTvisVisualization.cpp
        TESTvisImageWriter.cpp)
ADD_EXECUTABLE(runtests ${TEST_SRC})
add_test(NAME visTests COMMAND runtests --gtest_output=xml:tests.xml
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
TARGET_LINK_LIBRARIES(runtests gtest gtest_main Visvid::Visvid)
gtest_add_tests(TARGET runtests )
