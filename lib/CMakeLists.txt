#set(headers )
#list(APPEND headers
#        visImageWriter.h visFrame.h visResult.h visTypes.h visUtils.h visVersion.h visvid.h visView.h visVisualization.h)
#message(WARNING "d=${headers}")
set(libvisvid
        visFrame.c
        visResult.c
        visBuffer.c
        visView.c
        visVisualization.c
        visImageWriter.c
        visVersion.h.in
        utils.c
#                ${headers}
        )

configure_file("${PROJECT_SOURCE_DIR}/lib/visVersion.h.in"
        "${PROJECT_BINARY_DIR}/lib/visVersion.h")


add_library(visvid ${libvisvid})
#set_target_properties(visvid PROPERTIES PUBLIC_HEADER "${headers}")
#target_link_libraries(visvid PUBLIC ${PROJECT_SOURCE_DIR}/lib/)
target_compile_features(visvid PRIVATE c_std_11)
target_compile_definitions(visvid PRIVATE WINDOWS_EXPORT_ALL_SYMBOLS=1)

if (VISVID_MEMDEBUG)
    MESSAGE(WARNING "MEMORY debug messages turned on")
    target_compile_definitions(visvid PRIVATE MEMORY_DEBUG)
endif ()

target_include_directories(visvid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(visvid PUBLIC ${CMAKE_BINARY_DIR}/lib/)
#
include(GenerateExportHeader)
generate_export_header(visvid)
install(TARGETS visvid EXPORT visvidTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
#        PUBLIC_HEADER DESTINATION include
        INCLUDES DESTINATION include
#        COMPONENT LIBVISVID
        )
#
#install(EXPORT visvidTargets
#        FILE visvidTargets.cmake
#        NAMESPACE visvid::
#        DESTINATION lib/cmake/visvid
#        )