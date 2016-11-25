include(GenerateExportHeader)
generate_export_header(visvid)
set_property(TARGET visvid PROPERTY ${LIBVISVID_VERSION})


#install(TARGETS visvid EXPORT visvidTargets
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin
#        INCLUDES DESTINATION include
#        )

install(
        FILES
        "${PROJECT_BINARY_DIR}/lib/visVersion.h"
        "${CMAKE_CURRENT_BINARY_DIR}/lib/visvid_export.h"
        DESTINATION
        include
        COMPONENT
        Devel
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/visvid/LibVisVidConfigVersion.cmake"
        VERSION ${LIBVISVID_VERSION}
        COMPATIBILITY AnyNewerVersion
)


#export(EXPORT visvidTargets
#        FILE "${CMAKE_CURRENT_BINARY_DIR}/visvid/visvidTargetsTargets.cmake"
#        #        NAMESPACE Upstream::
#        )
