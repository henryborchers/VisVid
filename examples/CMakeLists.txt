add_executable(make_image make_image.c)
target_link_libraries(make_image PRIVATE Visvid::Visvid ${FFMPEG_LIBRARIES})
target_include_directories(make_image PRIVATE ${FFMPEG_INCLUDE_DIR})
