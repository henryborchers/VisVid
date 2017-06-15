find_package(Doxygen)

set(root ${PROJECT_SOURCE_DIR}/lib)
set(DOC_OUTPUT ${CMAKE_BINARY_DIR}/documentation)

if(VISVID_BUILDDOCS)
    if (DOXYGEN_FOUND)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating documentation" VERBATIM)

        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/documentation/html
                DESTINATION share/doc
                COMPONENT Documentation)
    endif(DOXYGEN_FOUND)
endif(VISVID_BUILDDOCS)