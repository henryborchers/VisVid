find_package(Doxygen)

#set(mainpage ${CMAKE_CURRENT_SOURCE_DIR}/documentation/mainpage.md)
set(root ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(DOC_OUTPUT ${CMAKE_BINARY_DIR}/documentation)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating documentation" VERBATIM)


endif ()