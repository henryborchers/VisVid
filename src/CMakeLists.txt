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

set(FOUND_LIBRARIES ON)

if (NOT FFMPEG_FOUND)
    set(FOUND_LIBRARIES OFF)
    message(WARNING "Unable to find FFMPEG")
endif ()

if (NOT SDL2_FOUND)
    set(FOUND_LIBRARIES OFF)
    message(WARNING "Unable to find SDL2")
endif ()

if (FOUND_LIBRARIES)
    add_executable(CreateVisual ${CreateVisual_src})
    #    target_include_directories(CreateVisual PRIVATE ${PROJECT_SOURCE_DIR}/lib)
    target_include_directories(CreateVisual PRIVATE ${SDL2_INCLUDE_DIR})
    target_include_directories(CreateVisual PRIVATE ${FFMPEG_INCLUDE_DIR})
    target_link_libraries(CreateVisual Visvid::Visvid)
    target_link_libraries(CreateVisual ${FFMPEG_LIBRARIES})
    target_link_libraries(CreateVisual ${SDL2_LIBRARIES})
    # Contifigure the optional Dialog box

    if (VISVID-DEMO_NOC_FILE_DIALOG)
        add_subdirectory(thirdpartylibs)
        set(DialogBox_src filedialog.c filedialog.h)
        add_library(DialogBox STATIC ${DialogBox_src})
        target_link_libraries(DialogBox NOC_FILE_DIALOG)
        target_link_libraries(CreateVisual DialogBox)
    endif (VISVID-DEMO_NOC_FILE_DIALOG)

    install(TARGETS CreateVisual
            RUNTIME DESTINATION bin
            COMPONENT Application)

else ()
    message(WARNING "Missing required libraries. Unable to build CreateVisual")
endif ()


