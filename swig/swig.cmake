find_package(SWIG REQUIRED)
message(STATUS "Found SWIG")

INCLUDE(${SWIG_USE_FILE})


FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

SET(CMAKE_SWIG_FLAGS "")
SWIG_ADD_MODULE(visVid python swig/visFrame.i lib/visFrame.c)
SWIG_LINK_LIBRARIES(visVid ${PYTHON_LIBRARIES})
