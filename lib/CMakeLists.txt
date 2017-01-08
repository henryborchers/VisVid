set(libvisvid
        visFrame.c visFrame.h
        visResult.c visResult.h
        visBuffer.c visBuffer.h
        visView.c visView.h
        visVisualization.c visVisualization.h
        visImageWriter.c visImageWriter.h
        visTypes.h
        visVersion.h.in
        visvid.h
        visUtils.h)
configure_file("${PROJECT_SOURCE_DIR}/lib/visVersion.h.in"
        "${PROJECT_SOURCE_DIR}/lib/visVersion.h")

add_library(visvidStatic STATIC ${libvisvid})
add_library(visvid SHARED ${libvisvid})

install(TARGETS visvid visvidStatic
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        COMPONENT LIBVISVID)