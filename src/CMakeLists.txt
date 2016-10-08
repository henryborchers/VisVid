set(CreateVisual_src createVisual.c)
add_executable(CreateVisual ${CreateVisual_src})
target_include_directories(CreateVisual PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(CreateVisual visvid)
