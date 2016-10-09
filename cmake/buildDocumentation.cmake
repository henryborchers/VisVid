find_package(Doxygen)

#set(mainpage ${CMAKE_CURRENT_SOURCE_DIR}/documentation/mainpage.md)
#set(root ${CMAKE_CURRENT_SOURCE_DIR}/include)
if (DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating documentation" VERBATIM)


endif ()