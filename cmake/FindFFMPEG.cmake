#[=======================================================================[.rst:

FindFFMPEG
----------

This will define the following variables::

  FFMPEG_FOUND       - True if the system has the FFMpeg library
  FFMPEG_VERSION     - The version of the FFmpeg library which was found
  FFMPEG_EXECUTABLE  - The path to the ffmpeg executable which was found
  FFMPEG_INCLUDE_DIR - The path where the headers are located. avcodec.h, avformat.h ...

and the following imported targets::

  FFmpeg::ffmpeg   - The ffmpeg executable
  FFmpeg::ffprobe  - The ffprobe executable
  FFmpeg::avcodec  - The libavcodec library
  FFmpeg::avformat - The libavformat library
  FFmpeg::avutil   - The libavutil library
  FFmpeg::swscale  - The swscale library
  FFmpeg::avdevice - The avdevice library

#]=======================================================================]
include_guard(GLOBAL)

macro(FFMPEG_ERROR)
    set(FFmpeg_FOUND FALSE)
    set(FFMPEG_FOUND FALSE)
    if (FFMPEG_FIND_REQUIRED)
        message(FATAL_ERROR "no found")
    endif ()
endmacro(FFMPEG_ERROR)

find_program(FFMPEG_EXECUTABLE NAMES ffmpeg)
if(NOT TARGET FFmpeg::ffmpeg)
    add_executable(FFmpeg::ffmpeg IMPORTED)
    set_target_properties(FFmpeg::ffmpeg PROPERTIES IMPORTED_LOCATION ${FFMPEG_EXECUTABLE})
endif(NOT TARGET FFmpeg::ffmpeg)

find_program(FFMPEG_FFPROBE_EXECUTABLE NAMES ffprobe)

if(NOT TARGET FFmpeg::probe)
    add_executable(FFmpeg::probe IMPORTED)
    set_target_properties(FFmpeg::probe PROPERTIES IMPORTED_LOCATION ${FFMPEG_FFPROBE_EXECUTABLE})
endif(NOT TARGET FFmpeg::probe)
# TODO: set library versionsThe path

if(FFMPEG_EXECUTABLE)
    execute_process(COMMAND ${FFMPEG_EXECUTABLE} -version
            OUTPUT_VARIABLE FFMPEG_VERSION)
    string(REGEX MATCH "[0-9]\\.[0-9]*\\.[0-9]*-[0-9]*" FFMPEG_VERSION ${FFMPEG_VERSION})
endif()


if ("avformat" IN_LIST FFMPEG_FIND_COMPONENTS)
    find_path(FFMPEG_AVFORMAT_INCLUDE_DIR libavformat/avformat.h
            HINTS
                ${FFMPEG_DIR}
                ${FFMPEG_DIR}/include
                $ENV{FFMPEG_DIR}
                $ENV{FFMPEG_DIR}/ffmpeg
                $ENV{FFMPEG_DIR}/libavformat
                $ENV{FFMPEG_DIR}/include/libavformat
                $ENV{FFMPEG_DIR}/include/ffmpeg
                /usr/local/include/ffmpeg
                /usr/include/ffmpeg
                /usr/include/libavformat
                /usr/include/ffmpeg/libavformat
                /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libavformat
                /usr/local/include/libavformat
            )
    mark_as_advanced(FFMPEG_AVFORMAT_INCLUDE_DIR)

    find_library(FFMPEG_AVFORMAT_LIBRARY
            NAMES
                avformat
                libavformat
            HINTS
                ${CONAN_FFMPEG_ROOT}/bin
                ${FFMPEG_DIR}/lib
                $ENV{FFMPEG_DIR}
                $ENV{FFMPEG_DIR}/lib
                $ENV{FFMPEG_DIR}/libavformat
                /usr/local/lib
                /usr/lib
            )
    mark_as_advanced(FFMPEG_AVFORMAT_LIBRARY)
    add_library(FFmpeg::avformat UNKNOWN IMPORTED)
    set_target_properties(FFmpeg::avformat PROPERTIES
        IMPORTED_LOCATION ${FFMPEG_AVFORMAT_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_AVFORMAT_INCLUDE_DIR}
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        )
    set(FFMPEG_avformat_FOUND TRUE)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_AVFORMAT_LIBRARY})
    list(APPEND FFMPEG_INCLUDE_DIR ${FFMPEG_AVFORMAT_INCLUDE_DIR})

endif ()

# ###################################

if ("avcodec" IN_LIST FFMPEG_FIND_COMPONENTS)
    find_path(FFMPEG_AVCODEC_INCLUDE_DIR libavcodec/avcodec.h
            HINTS
                ${FFMPEG_DIR}
                ${FFMPEG_DIR}/include
                $ENV{FFMPEG_DIR}
                $ENV{FFMPEG_DIR}/ffmpeg
                $ENV{FFMPEG_DIR}/libavcodec
                $ENV{FFMPEG_DIR}/include/libavcodec
                $ENV{FFMPEG_DIR}/include/ffmpeg
                /usr/local/include/ffmpeg
                /usr/include/ffmpeg
                /usr/include/libavcodec
                /usr/include/ffmpeg/libavcodec
                /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libavcodec
                /usr/local/include/libavcodec
            )
    mark_as_advanced(FFMPEG_AVCODEC_INCLUDE_DIR)

    find_library(FFMPEG_AVCODEC_LIBRARY
            NAMES
                avcodec
                libavcodec
            HINTS
                ${CONAN_FFMPEG_ROOT}/bin
                ${FFMPEG_DIR}/lib
                $ENV{FFMPEG_DIR}
                $ENV{FFMPEG_DIR}/lib
                $ENV{FFMPEG_DIR}/libavcodec
                /usr/local/lib
                /usr/lib
            )
    mark_as_advanced(FFMPEG_AVCODEC_LIBRARY)

    add_library(FFmpeg::avcodec UNKNOWN IMPORTED)
    set_target_properties(FFmpeg::avcodec PROPERTIES
            IMPORTED_LOCATION ${FFMPEG_AVCODEC_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_AVCODEC_INCLUDE_DIR}
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"


            )
    set(FFMPEG_avcodec_FOUND TRUE)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_AVCODEC_LIBRARY})
    list(APPEND FFMPEG_INCLUDE_DIR ${FFMPEG_AVCODEC_INCLUDE_DIR})

endif()


# ###################################

if ("avutil" IN_LIST FFMPEG_FIND_COMPONENTS)
    find_path(FFMPEG_AVUTIL_INCLUDE_DIR libavutil/avutil.h
        HINTS
            ${FFMPEG_DIR}
            ${FFMPEG_DIR}/include
            $ENV{FFMPEG_DIR}
            $ENV{FFMPEG_DIR}/ffmpeg
            $ENV{FFMPEG_DIR}/libavutil
            $ENV{FFMPEG_DIR}/include/libavutil
            $ENV{FFMPEG_DIR}/include/ffmpeg
            /usr/local/include/ffmpeg
            /usr/include/ffmpeg
            /usr/include/libavutil
            /usr/include/ffmpeg/libavutil
            /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libavutil
            /usr/local/include/libavutil
            )
    mark_as_advanced(FFMPEG_AVUTIL_INCLUDE_DIR)

    find_library(FFMPEG_AVUTIL_LIBRARY
        NAMES
            avutil
            libavutil
        HINTS
            ${CONAN_FFMPEG_ROOT}/bin
            ${FFMPEG_DIR}/lib
            $ENV{FFMPEG_DIR}
            $ENV{FFMPEG_DIR}/lib
            $ENV{FFMPEG_DIR}/libavutil
            /usr/local/lib
            /usr/lib
            )
    mark_as_advanced(FFMPEG_AVUTIL_LIBRARY)

    add_library(FFmpeg::avutil UNKNOWN IMPORTED)
    set_target_properties(FFmpeg::avutil PROPERTIES
            IMPORTED_LOCATION ${FFMPEG_AVUTIL_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_AVUTIL_INCLUDE_DIR}
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"

            )
    set(FFMPEG_avutil_FOUND TRUE)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_AVUTIL_LIBRARY})
    list(APPEND FFMPEG_INCLUDE_DIR ${FFMPEG_AVUTIL_INCLUDE_DIR})

endif ()

# ###################################


if ("avutil" IN_LIST FFMPEG_FIND_COMPONENTS)
    find_path(FFMPEG_SWSCALE_INCLUDE_DIR libswscale/swscale.h
        HINTS
            ${CONAN_FFMPEG_ROOT}/bin
            ${FFMPEG_DIR}
            ${FFMPEG_DIR}/include
            $ENV{FFMPEG_DIR}
            $ENV{FFMPEG_DIR}/ffmpeg
            $ENV{FFMPEG_DIR}/libswscale
            $ENV{FFMPEG_DIR}/include/libswscale
            $ENV{FFMPEG_DIR}/include/ffmpeg
            /usr/local/include/ffmpeg
            /usr/include/ffmpeg
            /usr/include/libswscale
            /usr/include/ffmpeg/libswscale
            /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libswscale
            /usr/local/include/libswscale
            )
    mark_as_advanced(FFMPEG_SWSCALE_INCLUDE_DIR)

    find_library(FFMPEG_SWSCALE_LIBRARY
        NAMES
            swscale
            libswscale
        HINTS
            ${FFMPEG_DIR}/lib
            ${CONAN_FFMPEG_ROOT}/bin
            $ENV{FFMPEG_DIR}
            $ENV{FFMPEG_DIR}/lib
            $ENV{FFMPEG_DIR}/libswscale
            /usr/local/lib
            /usr/lib
            )

    mark_as_advanced(FFMPEG_SWSCALE_LIBRARY)

    add_library(FFmpeg::swscale UNKNOWN IMPORTED)
    set_target_properties(FFmpeg::swscale PROPERTIES
            IMPORTED_LOCATION ${FFMPEG_SWSCALE_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_SWSCALE_INCLUDE_DIR}
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            )
    set(FFMPEG_swscale_FOUND TRUE)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_SWSCALE_LIBRARY})
    list(APPEND FFMPEG_INCLUDE_DIR ${FFMPEG_SWSCALE_INCLUDE_DIR})

endif()
# ###################################

if ("avutil" IN_LIST FFMPEG_FIND_COMPONENTS)
    find_library(FFMPEG_AVDEVICE_LIBRARY
        NAMES
            avdevice
            libavdevice
        HINTS
            ${CONAN_FFMPEG_ROOT}/bin
            ${FFMPEG_DIR}/lib
            $ENV{FFMPEG_DIR}
            $ENV{FFMPEG_DIR}/lib
            $ENV{FFMPEG_DIR}/libavdevice
            /usr/local/lib
            /usr/lib
            )
    mark_as_advanced(FFMPEG_AVDEVICE_LIBRARY)

    find_path(FFMPEG_AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h
        HINTS
            ${FFMPEG_DIR}
            ${FFMPEG_DIR}/include
            $ENV{FFMPEG_DIR}
            $ENV{FFMPEG_DIR}/ffmpeg
            $ENV{FFMPEG_DIR}/libavdevice
            $ENV{FFMPEG_DIR}/include/libavdevice
            $ENV{FFMPEG_DIR}/include/ffmpeg
            /usr/local/include/ffmpeg
            /usr/include/ffmpeg
            /usr/include/libavdevice
            /usr/include/ffmpeg/libavdevice
            /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libavdevice
            /usr/local/include/libavdevice
            )
    mark_as_advanced(FFMPEG_AVDEVICE_INCLUDE_DIR)

    add_library(FFmpeg::avdevice UNKNOWN IMPORTED)
    set_target_properties(FFmpeg::avdevice PROPERTIES
            IMPORTED_LOCATION ${FFMPEG_AVDEVICE_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${FFMPEG_AVDEVICE_INCLUDE_DIR}
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            )
    set(FFMPEG_avdevice_FOUND TRUE)
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_AVDEVICE_LIBRARY})
    list(APPEND FFMPEG_INCLUDE_DIR ${FFMPEG_AVDEVICE_INCLUDE_DIR})

endif ()

#===================================
list(REMOVE_DUPLICATES FFMPEG_INCLUDE_DIR)
include(FindPackageHandleStandardArgs)
#message(WARNING "FFMPEG_FIND_COMPONENTS ${FFMPEG_FIND_COMPONENTS}")

find_package_handle_standard_args(FFMPEG
        REQUIRED_VARS
            FFMPEG_LIBRARIES
            FFMPEG_INCLUDE_DIR
        VERSION_VAR FFMPEG_VERSION
        HANDLE_COMPONENTS
        )


#find_package_handle_standard_args(FFMPEG
#        DEFAULT_MSG
#        FFMPEG_AVFORMAT_LIBRARY
#        FFMPEG_AVFORMAT_INCLUDE_DIR
#        FFMPEG_AVCODEC_LIBRARY
#        FFMPEG_AVCODEC_INCLUDE_DIR
#        FFMPEG_AVUTIL_LIBRARY
#        FFMPEG_AVUTIL_INCLUDE_DIR
#        FFMPEG_LIBRARIES
#        FFMPEG_INCLUDE_DIR
#        )
#
#


#set(FFMPEG_FOUND "YES")
#        VERSION_VAR AVFORMAT_VERSION_STRING)
##.rst:
## FindFFMPEG
## ----------
##
## Find the native FFMPEG includes and library
##
## This module defines::
##
##  FFMPEG_INCLUDE_DIR, where to find avcodec.h, avformat.h ...
##  FFMPEG_LIBRARIES, the libraries to link against to use FFMPEG.
##  FFMPEG_FOUND, If false, do not try to use FFMPEG.
##
## also defined, but not for general use are::
##
##   FFMPEG_AVFORMAT_LIBRARY, where to find the FFMPEG avformat library.
##   FFMPEG_AVCODEC_LIBRARY, where to find the FFMPEG avcodec library.
##
## This is useful to do it this way so that we can always add more libraries
## if needed to ``FFMPEG_LIBRARIES`` if ffmpeg ever changes...
#
##=============================================================================
## Copyright: 1993-2008 Ken Martin, Will Schroeder, Bill Lorensen
##
## Distributed under the OSI-approved BSD License (the "License");
## see accompanying file Copyright.txt for details.
##
## This software is distributed WITHOUT ANY WARRANTY; without even the
## implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the License for more information.
##=============================================================================
## (To distribute this file outside of YCM, substitute the full
##  License text for the above reference.)
#
## Originally from VTK project
#
#
##set(ENV{FFMPEG_DIR} "${FFMPEG_DIR}")
#find_path(FFMPEG_INCLUDE_DIR1 avformat.h HINTS
#        ${FFMPEG_DIR}
#        ${FFMPEG_DIR}/include/libavformat
#        $ENV{FFMPEG_DIR}
#        $ENV{FFMPEG_DIR}/ffmpeg
#        $ENV{FFMPEG_DIR}/libavformat
#        $ENV{FFMPEG_DIR}/include/libavformat
#        $ENV{FFMPEG_DIR}/include/ffmpeg
#        /usr/local/include/ffmpeg
#        /usr/include/ffmpeg
#        /usr/include/libavformat
#        /usr/include/ffmpeg/libavformat
#        /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libavformat
#        /usr/local/include/libavformat
#)
#
#find_path(FFMPEG_INCLUDE_DIR2 avutil.h
#  $ENV{FFMPEG_DIR}
#  $ENV{FFMPEG_DIR}/ffmpeg
#  $ENV{FFMPEG_DIR}/libavutil
#  $ENV{FFMPEG_DIR}/include/libavutil
#  $ENV{FFMPEG_DIR}/include/ffmpeg
#  /usr/local/include/ffmpeg
#  /usr/include/ffmpeg
#  /usr/include/libavutil
#  /usr/include/ffmpeg/libavutil
#  /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libavutil
#  /usr/local/include/libavutil
#)
#
#find_path(FFMPEG_INCLUDE_DIR3 avcodec.h
#  $ENV{FFMPEG_DIR}
#  $ENV{FFMPEG_DIR}/ffmpeg
#  $ENV{FFMPEG_DIR}/libavcodec
#  $ENV{FFMPEG_DIR}/include/libavcodec
#  $ENV{FFMPEG_DIR}/include/ffmpeg
#  /usr/local/include/ffmpeg
#  /usr/include/ffmpeg
#  /usr/include/libavcodec
#  /usr/include/ffmpeg/libavcodec
#  /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libavcodec
#  /usr/local/include/libavcodec
#)
#
#find_path(FFMPEG_INCLUDE_DIR4 swscale.h
#  $ENV{FFMPEG_DIR}
#  $ENV{FFMPEG_DIR}/ffmpeg
#  $ENV{FFMPEG_DIR}/libswscale
#  $ENV{FFMPEG_DIR}/include/libswscale
#  $ENV{FFMPEG_DIR}/include/ffmpeg
#  /usr/local/include/ffmpeg
#  /usr/include/ffmpeg
#  /usr/include/libswscale
#  /usr/include/ffmpeg/libswscale
#  /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libswscale
#  /usr/local/include/libswscale
#)
#
#find_path(FFMPEG_INCLUDE_DIR5 avdevice.h
#  $ENV{FFMPEG_DIR}
#  $ENV{FFMPEG_DIR}/ffmpeg
#  $ENV{FFMPEG_DIR}/libavdevice
#  $ENV{FFMPEG_DIR}/include/libavdevice
#  $ENV{FFMPEG_DIR}/include/ffmpeg
#  /usr/local/include/ffmpeg
#  /usr/include/ffmpeg
#  /usr/include/libavdevice
#  /usr/include/ffmpeg/libavdevice
#  /usr/include/${CMAKE_LIBRARY_ARCHITECTURE}/libavdevice
#  /usr/local/include/libavdevice
#)
#
#if(FFMPEG_INCLUDE_DIR1)
#  if(FFMPEG_INCLUDE_DIR2)
#    if(FFMPEG_INCLUDE_DIR3)
#      set(FFMPEG_INCLUDE_DIR ${FFMPEG_INCLUDE_DIR1}
#                             ${FFMPEG_INCLUDE_DIR2}
#                             ${FFMPEG_INCLUDE_DIR3})
#    endif()
#  endif()
#endif()
#
#if(FFMPEG_INCLUDE_DIR4)
#  set(FFMPEG_INCLUDE_DIR ${FFMPEG_INCLUDE_DIR}
#                         ${FFMPEG_INCLUDE_DIR4})
#endif()
#
#if(FFMPEG_INCLUDE_DIR5)
#  set(FFMPEG_INCLUDE_DIR ${FFMPEG_INCLUDE_DIR}
#                         ${FFMPEG_INCLUDE_DIR5}
#                         ${FFMPEG_INCLUDE_DIR5}/..)
#endif()
#
#find_library(FFMPEG_AVFORMAT_LIBRARY avformat
#  $ENV{FFMPEG_DIR}
#  $ENV{FFMPEG_DIR}/lib
#  $ENV{FFMPEG_DIR}/libavformat
#  /usr/local/lib
#  /usr/lib
#)
#
#find_library(FFMPEG_AVCODEC_LIBRARY avcodec
#  $ENV{FFMPEG_DIR}
#  $ENV{FFMPEG_DIR}/lib
#  $ENV{FFMPEG_DIR}/libavcodec
#  /usr/local/lib
#  /usr/lib
#)
#
#find_library(FFMPEG_AVUTIL_LIBRARY avutil
#  $ENV{FFMPEG_DIR}
#  $ENV{FFMPEG_DIR}/lib
#  $ENV{FFMPEG_DIR}/libavutil
#  /usr/local/lib
#  /usr/lib
#)
#
#if(NOT DISABLE_SWSCALE)
#  find_library(FFMPEG_SWSCALE_LIBRARY swscale
#    $ENV{FFMPEG_DIR}
#    $ENV{FFMPEG_DIR}/lib
#    $ENV{FFMPEG_DIR}/libswscale
#    /usr/local/lib
#    /usr/lib
#  )
#endif(NOT DISABLE_SWSCALE)
#
#find_library(FFMPEG_AVDEVICE_LIBRARY avdevice
#  $ENV{FFMPEG_DIR}
#  $ENV{FFMPEG_DIR}/lib
#  $ENV{FFMPEG_DIR}/libavdevice
#  /usr/local/lib
#  /usr/lib
#)
#
#find_library(_FFMPEG_z_LIBRARY_ z
#  $ENV{FFMPEG_DIR}
#  $ENV{FFMPEG_DIR}/lib
#  /usr/local/lib
#  /usr/lib
#)
#
#
#
#if(FFMPEG_INCLUDE_DIR)
#  if(FFMPEG_AVFORMAT_LIBRARY)
#    if(FFMPEG_AVCODEC_LIBRARY)
#      if(FFMPEG_AVUTIL_LIBRARY)
#        set(FFMPEG_FOUND "YES")
#        set(FFMPEG_LIBRARIES ${FFMPEG_AVFORMAT_LIBRARY}
#                             ${FFMPEG_AVCODEC_LIBRARY}
#                             ${FFMPEG_AVUTIL_LIBRARY}
#          )
#        if(FFMPEG_SWSCALE_LIBRARY)
#          set(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES}
#                               ${FFMPEG_SWSCALE_LIBRARY}
#          )
#        endif()
#        if(FFMPEG_AVDEVICE_LIBRARY)
#          set(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES}
#                               ${FFMPEG_AVDEVICE_LIBRARY}
#          )
#        endif()
#        if(_FFMPEG_z_LIBRARY_)
#          set( FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES}
#                                ${_FFMPEG_z_LIBRARY_}
#          )
#        endif()
#      endif()
#    endif()
#  endif()
#endif()
#
#mark_as_advanced(
#  FFMPEG_INCLUDE_DIR
#  FFMPEG_INCLUDE_DIR1
#  FFMPEG_INCLUDE_DIR2
#  FFMPEG_INCLUDE_DIR3
#  FFMPEG_INCLUDE_DIR4
#  FFMPEG_INCLUDE_DIR5
#  FFMPEG_AVFORMAT_LIBRARY
#  FFMPEG_AVCODEC_LIBRARY
#  FFMPEG_AVUTIL_LIBRARY
#  FFMPEG_SWSCALE_LIBRARY
#  FFMPEG_AVDEVICE_LIBRARY
#  _FFMPEG_z_LIBRARY_
#  )
#
## Set package properties if FeatureSummary was included
#if(COMMAND set_package_properties)
#  set_package_properties(FFMPEG PROPERTIES DESCRIPTION "A complete, cross-platform solution to record, convert and stream audio and video")
#  set_package_properties(FFMPEG PROPERTIES URL "http://ffmpeg.org/")
#endif()
