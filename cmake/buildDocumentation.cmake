find_package(Doxygen)


if(VISVID_BUILDDOCS AND DOXYGEN_FOUND)
        doxygen_add_docs(documentation ${PROJECT_SOURCE_DIR}/lib
                COMMENT "Generated html pages"
                DOXYGEN_HAVE_DOT YES
                )
#        set(root ${PROJECT_SOURCE_DIR}/lib)
#        set(DOC_OUTPUT ${CMAKE_BINARY_DIR}/documentation)
#        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)


#        add_custom_target(documentation ALL
#                DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/documentation/html
#                )
#        add_custom_command(
#                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/documentation/html
#                COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
#                COMMENT "Generating documentation"
#
#        )

#        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/documentation/html
#                DESTINATION share/doc
#                COMPONENT Documentation)
endif()