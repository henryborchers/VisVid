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

if(VISVID_NOC_FILE_DIALOG)
include(ExternalProject)
ExternalProject_Add(NOC_FILE_DIALOG
        GIT_REPOSITORY "https://github.com/guillaumechereau/noc.git"
        GIT_TAG "master"
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdpartylibs/noc"
        INSTALL_COMMAND ""

        )

endif()


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
    target_include_directories(CreateVisual PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdpartylibs)

    if(APPLE)
        find_library(cocoa_lib Cocoa)
        add_definitions(-DNOC_FILE_DIALOG_OSX)
        target_compile_options(CreateVisual PRIVATE "-ObjC")
        target_link_libraries(CreateVisual ${cocoa_lib})
    elseif(WIN32)
        add_definitions(-DNOC_FILE_DIALOG_WIN32)
    else()
        message(FATAL_ERROR "Cannot use NOC file dialog library due to unsupported system")
    endif()

    add_dependencies(CreateVisual NOC_FILE_DIALOG)
endif(VISVID_NOC_FILE_DIALOG)