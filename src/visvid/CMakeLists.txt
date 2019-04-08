configure_file("visVersion.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/visvid/visVersion.h")


add_library(Visvid
    visFrame.c
    visResult.c
    visBuffer.c
    visView.c
    visVisualization.c
    visImageWriter.c
        )

add_library(Visvid::Visvid ALIAS Visvid)
target_compile_features(Visvid PRIVATE c_std_11)
list(APPEND VISPUBLIC_HEADER include/visvid/visvid.h)
list(APPEND VISPUBLIC_HEADER include/visvid/utils.h)
list(APPEND VISPUBLIC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/visvid/visVersion.h")

set_target_properties(Visvid PROPERTIES
        C_STANDARD 11
        PUBLIC_HEADER "${VISPUBLIC_HEADER}"
        ENABLE_EXPORTS NO
        )


target_include_directories(Visvid PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
# todo: test for floorf and if not avaliable without adding m, then link it
target_link_libraries(Visvid PRIVATE m)
include(GenerateExportHeader)
generate_export_header(Visvid)

install(TARGETS Visvid EXPORT VisvidTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/visvid
        INCLUDES DESTINATION include
        )
#
install(EXPORT VisvidTargets
        FILE VisvidTargets.cmake
        NAMESPACE Visvid::
        DESTINATION share/cmake/${PROJECT_NAME}
        )