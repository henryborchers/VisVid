find_package(Doxygen)


if(DOXYGEN_FOUND)
        doxygen_add_docs(documentation visvid
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src/visvid/include
                COMMENT "Generated html pages"
                DOXYGEN_HAVE_DOT YES
                )
endif()