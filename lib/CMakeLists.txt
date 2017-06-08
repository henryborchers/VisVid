set(libvisvid
        visvid.h
        visFrame.c visFrame.h
        visResult.c visResult.h
        visBuffer.c visBuffer.h
        visView.c visView.h
        visVisualization.c visVisualization.h
        visImageWriter.c visImageWriter.h
        visTypes.h
        visVersion.h.in
        visUtils.h
        utils.c
        )

configure_file("${PROJECT_SOURCE_DIR}/lib/visVersion.h.in"
        "${PROJECT_SOURCE_DIR}/lib/visVersion.h")

if(VISVID_MEMDEBUG)
    MESSAGE(WARNING "MEMORY debug messages turned on")
    add_definitions(-DMEMORY_DEBUG)
endif()

add_library(visvid ${libvisvid})
target_link_libraries(visvid PUBLIC ${PROJECT_SOURCE_DIR}/lib/)
target_compile_features(visvid PUBLIC c_std_11)
target_compile_definitions(visvid PUBLIC WINDOWS_EXPORT_ALL_SYMBOLS=1)
include(GenerateExportHeader)
generate_export_header(visvid)
install(TARGETS visvid EXPORT visvidTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
        COMPONENT LIBVISVID)