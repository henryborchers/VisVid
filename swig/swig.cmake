find_package(SWIG REQUIRED)
message(STATUS "Found SWIG")

INCLUDE(${SWIG_USE_FILE})


FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

SET(CMAKE_SWIG_FLAGS "-py3")
SWIG_ADD_MODULE(visVid python swig/visVid.i lib/visFrame.c lib/visBuffer.c lib/visResult.c
        lib/visView.c lib/visView.h
        lib/visVisualization.c lib/visVisualization.h
        lib/visTypes.h)
SWIG_LINK_LIBRARIES(visVid ${PYTHON_LIBRARIES})
