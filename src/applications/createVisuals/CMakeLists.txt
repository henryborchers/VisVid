include(FetchContent)
if(NOT SDL2_DIR)
    FetchContent_Declare(
        sdl2
        URL https://libsdl.org/release/SDL2-2.0.8.tar.gz
    )
    FetchContent_GetProperties(sdl2)
    if(NOT sdl2_POPULATED)
        FetchContent_Populate(sdl2)
        add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()

endif(NOT SDL2_DIR)

FetchContent_Declare(
    noc
    GIT_REPOSITORY https://github.com/guillaumechereau/noc.git
)
FetchContent_GetProperties(noc)
if(NOT noc_POPULATED)
    FetchContent_Populate(noc)
    add_library(NOC_FILE_DIALOG INTERFACE)
    target_include_directories(NOC_FILE_DIALOG INTERFACE "${noc_SOURCE_DIR}")
    target_compile_definitions(NOC_FILE_DIALOG INTERFACE NOC_FILE_DIALOG_IMPLEMENTATION)
    if(APPLE)
        find_library(cocoa_lib Cocoa REQUIRED)
        target_compile_definitions(NOC_FILE_DIALOG INTERFACE NOC_FILE_DIALOG_OSX)
        target_compile_options(NOC_FILE_DIALOG INTERFACE "-ObjC")
        target_link_libraries(NOC_FILE_DIALOG INTERFACE ${cocoa_lib})
    elseif(WIN32)
        target_compile_definitions(NOC_FILE_DIALOG INTERFACE NOC_FILE_DIALOG_WIN32)
    elseif(UNIX)
        message(STATUS "Identified OS: Unix, Using GTK2")
        find_package(GTK2 REQUIRED gtk)
        target_compile_definitions(NOC_FILE_DIALOG INTERFACE NOC_FILE_DIALOG_GTK)
        target_include_directories(NOC_FILE_DIALOG INTERFACE ${GTK2_INCLUDE_DIRS})
        target_link_libraries(NOC_FILE_DIALOG INTERFACE ${GTK2_LIBRARIES})
    else()
        message(FATAL_ERROR "Cannot use NOC file dialog library due to unsupported system")
    endif()
endif()


#set(FOUND_LIBRARIES ON)
if (NOT FFMPEG_FOUND)
    set(FOUND_LIBRARIES OFF)
    message(WARNING "Unable to find FFMPEG")
endif ()

add_executable(CreateVisual
    main.c
    gui.c
    ramp.c
    videoDecoder.c
    simpleVideoPlayerUtils.c
    simpleVideoPlayer.c
    generator.c
    videoAndVisualPlayer.c
    ffmpeg_converter.c
    )

set_target_properties(CreateVisual PROPERTIES
    C_STANDARD 11
    )

target_include_directories(CreateVisual PRIVATE ${SDL2_INCLUDE_DIR})
target_include_directories(CreateVisual PRIVATE ${FFMPEG_INCLUDE_DIR})
target_link_libraries(CreateVisual
        PRIVATE
            Visvid::Visvid
            m
            ${FFMPEG_LIBRARIES}
            SDL2
#            SDL2::SDL2-static
#            ${SDL2_LIBRARIES}
        )

set(DialogBox_src filedialog.c filedialog.h)
add_library(DialogBox STATIC EXCLUDE_FROM_ALL ${DialogBox_src})
target_link_libraries(DialogBox NOC_FILE_DIALOG)
target_link_libraries(CreateVisual PRIVATE DialogBox)

