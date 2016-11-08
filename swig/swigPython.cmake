find_package(SWIG REQUIRED)
FIND_PACKAGE(PythonLibs)
if(VISVID_BuildPython3Bindings)
    message(STATUS "Building Python SWIG Bindings")

    INCLUDE(${SWIG_USE_FILE})


    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

    SET(CMAKE_SWIG_FLAGS "-py3")
    SWIG_ADD_MODULE(visVid python swig/visVid.i lib/visFrame.c lib/visBuffer.c lib/visResult.c
        lib/visView.c lib/visView.h
        lib/visVisualization.c lib/visVisualization.h
        lib/visTypes.h)
    SWIG_LINK_LIBRARIES(visVid ${PYTHON_LIBRARIES})
endif()
