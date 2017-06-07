set(CreateVisual_src main.c
        gui.c gui.h
        ramp.c ramp.h
        videoDecoder.c videoDecoder.h
        simpleVideoPlayerUtils.c simpleVideoPlayerUtils.h
        runPlayers.h
        simpleVideoPlayer.c simpleVideoPlayer.h
        generator.c generator.h
        videoAndVisualPlayer.c videoAndVisualPlayer.h
        ffmpeg_converter.c ffmpeg_converter.h)

find_package(SDL2 REQUIRED)
find_package(FFMPEG REQUIRED)
include_directories(${PROJECT_BINARY_DIR}/lib)

if(FFMPEG_FOUND AND SDL2_FOUND)
    add_executable(CreateVisual ${CreateVisual_src})
    target_include_directories(CreateVisual PRIVATE ${PROJECT_SOURCE_DIR}/lib)
    target_include_directories(CreateVisual PRIVATE ${SDL2_INCLUDE_DIR})
    target_include_directories(CreateVisual PRIVATE ${FFMPEG_INCLUDE_DIR})
    target_link_libraries(CreateVisual visvidStatic)
    target_link_libraries(CreateVisual ${FFMPEG_LIBRARIES})
    target_link_libraries(CreateVisual ${SDL2_LIBRARIES})

else()
    message(WARNING "SDL2 Not found")
endif()


# Contifigure the optional Dialog box

if(VISVID_NOC_FILE_DIALOG)
    add_subdirectory(thirdpartylibs)
    set(DialogBox_src filedialog.c filedialog.h)
    add_library(DialogBox STATIC ${DialogBox_src})
    target_link_libraries(DialogBox NOC_FILE_DIALOG)
    target_link_libraries(CreateVisual DialogBox)
endif(VISVID_NOC_FILE_DIALOG)
install(TARGETS CreateVisual
        RUNTIME DESTINATION bin
        COMPONENT Application)
