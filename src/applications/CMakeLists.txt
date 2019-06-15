#build application
option(VISVID_SAMPLE_CREATEVISUALS "Test Visualzations" OFF)
if (VISVID_SAMPLE_CREATEVISUALS)
    add_subdirectory(createVisuals)
endif (VISVID_SAMPLE_CREATEVISUALS)

option(VISVID_PYVISVID "Create python bindings" OFF)
if(VISVID_PYVISVID)
    add_subdirectory(pyvisvid)
endif(VISVID_PYVISVID)
