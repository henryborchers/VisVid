set(LIBRARY_VERSION_MAJOR 0)
set(LIBRARY_VERSION_MINOR 1)

set(libvisvid
        visFrame.c visFrame.h
        visResult.c visResult.h
        visBuffer.c visBuffer.h
        visView.c visView.h
        visVisualization.c visVisualization.h
        visTypes.h )
configure_file("${PROJECT_SOURCE_DIR}/lib/visVersion.h.in"
        "${PROJECT_BINARY_DIR}/lib/visVersion.h")

add_library(visvid ${libvisvid})