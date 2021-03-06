//
// Created by Borchers, Henry Samuel on 2019-04-11.
//


#include "Visualizer.h"
#include "generate.h"
#include "utils.h"
#include "visvid_exceptions.h"
extern  "C"{
    #include <libavcodec/version.h>
    #include <libavformat/avformat.h>
    #include <libavutil/version.h>
}

#include <pybind11/pybind11.h>

namespace py = pybind11;

void init(){
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
    av_register_all();
#endif
}

PYBIND11_MODULE(wrapper, m){
    pybind11::options options;
//    options.enable_function_signatures();
    options.disable_function_signatures();
//    options.disable_signature_sections();
    m.def("get_version", &get_version);
    m.def("initialize", &init);
    m.def("make_pgm", &make_pgm, py::arg("source"), py::arg("output"));
    py::register_exception<PyVisVidException>(m, "PyVisVidException");
}