find_package(Doxygen)


if(DOXYGEN_FOUND)
        set(DOXYGEN_EXAMPLE_PATH "${PROJECT_SOURCE_DIR}/src/applications")
        set(DOXYGEN_EXCLUDE "${PROJECT_SOURCE_DIR}/src/applications/createVisuals/thirdpartylibs")
        set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/docs/)
        doxygen_add_docs(documentation visvid ${PROJECT_SOURCE_DIR}/docs/mainpage.dox
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/visvid/include
                COMMENT "Generating ${PROJECT_NAME} api documentation"
                )
endif()