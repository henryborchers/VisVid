find_package(SWIG REQUIRED)
FIND_PACKAGE(PythonLibs 3)
if(VISVID_BuildPython3Bindings)
    message(STATUS "Building Python SWIG Bindings")

    INCLUDE(${SWIG_USE_FILE})


    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

        # FIXME: REMOVE THIS BOTTOM TO DYNAMICALLY FIND NUMPY
    INCLUDE_DIRECTORIES(/usr/local/lib/python3.5/site-packages/numpy/core/include)


    SET(CMAKE_SWIG_FLAGS "-py3")
    SWIG_ADD_MODULE(
            visVid
            python
            swig/visVid.i
            lib/visFrame.c
            lib/visBuffer.c
            lib/visResult.c
            lib/visView.c
            lib/visView.h
            lib/visVisualization.c
            lib/visVisualization.h
            lib/visTypes.h)
    SWIG_LINK_LIBRARIES(visVid ${PYTHON_LIBRARIES})

endif()
