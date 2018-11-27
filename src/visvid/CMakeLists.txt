configure_file("visVersion.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/visVersion.h")


add_library(Visvid
    visFrame.c
    visResult.c
    visBuffer.c
    visView.c
    visVisualization.c
    visImageWriter.c
    utils.c
    )

add_library(Visvid::Visvid ALIAS Visvid)
target_compile_features(Visvid PRIVATE c_std_11)

set_target_properties(Visvid PROPERTIES
        C_STANDARD 11
        PUBLIC_HEADER "visvid.h"
        )
if(MSVC)
    target_compile_definitions(Visvid PRIVATE WINDOWS_EXPORT_ALL_SYMBOLS=1)
endif(MSVC)

if (VISVID_MEMDEBUG)
    MESSAGE(WARNING "MEMORY debug messages turned on")
    target_compile_definitions(Visvid PRIVATE MEMORY_DEBUG)
endif ()

target_include_directories(Visvid PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
        )

include(GenerateExportHeader)
generate_export_header(Visvid)

install(TARGETS Visvid EXPORT VisvidTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/Visvid
        INCLUDES DESTINATION include
        )
#
install(EXPORT VisvidTargets
        FILE VisvidTargets.cmake
        NAMESPACE Visvid::
        DESTINATION lib/cmake/Visvid
        )