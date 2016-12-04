set(CreateVisual_src createVisual.c
        gui.c gui.h
        ramp.c ramp.h
        videoDecoder.c videoDecoder.h
        simpleVideoPlayerUtils.c simpleVideoPlayerUtils.h
        runPlayers.h
        simpleVideoPlayer.c simpleVideoPlayer.h
        generator.c generator.h
        videoAndVisualPlayer.c videoAndVisualPlayer.h)
find_package(SDL2 REQUIRED)
find_package(FFMPEG REQUIRED)
include_directories(${PROJECT_BINARY_DIR}/lib)


if(FFMPEG_FOUND AND SDL2_FOUND)
    add_executable(CreateVisual ${CreateVisual_src})
    target_include_directories(CreateVisual PRIVATE ${PROJECT_SOURCE_DIR}/lib)
    include_directories(${SDL2_INCLUDE_DIR})
    include_directories(${FFMPEG_INCLUDE_DIR})
    target_link_libraries(CreateVisual visvid)
    target_link_libraries(CreateVisual ${FFMPEG_LIBRARIES})
    target_link_libraries(CreateVisual ${SDL2_LIBRARIES})

else()
    message(WARNING "SDL2 Not found")
endif()

if(VISVID_NOC_FILE_DIALOG)
    add_definitions(-DNOC_FILE_DIALOG_IMPLEMENTATION)
    target_include_directories(CreateVisual PRIVATE ${PROJECT_SOURCE_DIR}/src/thirdpartylibs)

    if(APPLE)
        find_library(cocoa_lib Cocoa)
        add_definitions(-DNOC_FILE_DIALOG_OSX)
        target_compile_options(CreateVisual PRIVATE "-ObjC")
        target_link_libraries(CreateVisual ${cocoa_lib})
    else()
        message(WARNING "Cannot use NOC file dialog library due to unsupported system")
    endif(APPLE)

    # TODO: Add WINDOWS and linux versions
endif()