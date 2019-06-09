# TODO: this item uses unix style paths for locating python in it's venv. will not work for
#       windows, To fix this for windows, make sure that path "bin" is repplaced with scripts

include(FetchContent)
if(Python3_Interpreter_FOUND)
    FetchContent_Declare(libpybind11
            URL https://github.com/pybind/pybind11/archive/v2.2.3.tar.gz
            URL_HASH SHA1=0acaeb967f09dbef53a1e2a4366780548597beba
            )
    FetchContent_GetProperties(libpybind11)
    if (NOT libpybind11_POPULATED)
        FetchContent_Populate(libpybind11)
        add_subdirectory(${libpybind11_SOURCE_DIR} ${libpybind11_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()

    execute_process(COMMAND ${Python3_EXECUTABLE} -m venv ${PROJECT_BINARY_DIR}/venv)
    find_program(VENV_PYTHON
            NAMES
                python
            PATHS
                ${PROJECT_BINARY_DIR}/venv/bin
                ${PROJECT_BINARY_DIR}/venv/Scripts
            NO_DEFAULT_PATH
            NO_SYSTEM_ENVIRONMENT_PATH
            )
    execute_process(COMMAND ${VENV_PYTHON} -m pip install wheel)
    execute_process(COMMAND ${VENV_PYTHON} -m pip wheel --wheel-dir=${PROJECT_BINARY_DIR}/wheels -r ${CMAKE_SOURCE_DIR}/requirements-dev.txt)
    execute_process(COMMAND ${VENV_PYTHON} -m pip install --no-index --find-links=${PROJECT_BINARY_DIR}/wheels -r ${CMAKE_SOURCE_DIR}/requirements-dev.txt)

    add_custom_target(pyvisvid_build
        COMMAND
            ${VENV_PYTHON} setup.py
                build
                    --build-base ${CMAKE_CURRENT_BINARY_DIR}/build
                    --build-lib ${CMAKE_CURRENT_BINARY_DIR}/build/lib
                build_ext
                    --include-dirs=$<TARGET_FILE_DIR:Visvid::Visvid>:${PROJECT_SOURCE_DIR}/src/visvid/include:${libpybind11_SOURCE_DIR}/include
                    --library-dirs=$<TARGET_FILE_DIR:Visvid::Visvid>
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/build
        DEPENDS
            Visvid::Visvid
            )
    add_library(pyvisvid_module pyvisvid/visvid_extension.cpp pyvisvid/Visualizer.cpp pyvisvid/Visualizer.h)
    target_link_libraries(pyvisvid_module
            PRIVATE
                pybind11::module
                Visvid::Visvid
                ${FFMPEG_LIBRARIES}
            )
    target_include_directories(pyvisvid_module
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/..
            )

    add_custom_target(pyvisvid_run
            DEPENDS
                pyvisvid_build
            COMMAND ${VENV_PYTHON} -m pyvisvid
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build/lib
            )

    add_custom_target(pyvisvid_test
            DEPENDS
                pyvisvid_build
            COMMAND ${VENV_PYTHON} -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/tests/ --rootdir=${CMAKE_CURRENT_BINARY_DIR} --basetemp=${CMAKE_CURRENT_BINARY_DIR}/tmpdir
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build/lib
            )
    add_custom_target(pyvisvid_whl
        COMMAND
            ${VENV_PYTHON} setup.py
            build
                --build-base ${CMAKE_CURRENT_BINARY_DIR}/build
                --build-lib ${CMAKE_CURRENT_BINARY_DIR}/build/lib
            build_ext
                --include-dirs=$<TARGET_FILE_DIR:Visvid::Visvid>:${PROJECT_SOURCE_DIR}/src/visvid/include:${libpybind11_SOURCE_DIR}/include
                --library-dirs=$<TARGET_FILE_DIR:Visvid::Visvid>
            bdist_wheel
            --dist-dir ${PROJECT_BINARY_DIR}
            --bdist-dir ${CMAKE_CURRENT_BINARY_DIR}/bdist_wheel
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()