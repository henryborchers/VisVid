message(STATUS "Using swig.cmake")
set(VISVID_BuildPython3Bindings ON CACHE BOOL "Create SWIG Python bindings")
include(${CMAKE_SOURCE_DIR}/swig/swigPython.cmake)
