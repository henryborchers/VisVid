//
// Created by Borchers, Henry Samuel on 2019-04-11.
//


#include "Visualizer.h"


extern  "C"{
#include "shared/decode.h"
#include "shared/pgm.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <visvid/utils.h>
#include <visvid/visVersion.h>
#include <visvid/visvid.h>
}

#include <fstream>
#include <iostream>
#include <pybind11/pybind11.h>
#include <sstream>

#define MAX_BUFFER_SIZE 200
namespace py = pybind11;

enum pixel_component{
    Y,
    U,
    V,
};

size_t yuv_pixel_offset(AVFrame *frame, int x, int y, enum pixel_component component){
    size_t offset = 0;
    AVPixFmtDescriptor *desc = (AVPixFmtDescriptor *) av_pix_fmt_desc_get((AVPixelFormat)frame->format);
    signed int uvx = x >> desc->log2_chroma_w;
    signed int uvy = y >> desc->log2_chroma_h;

    switch(component){
        case Y:
            offset = frame->linesize[0] * y + x;
            break;
        case U:
            offset = frame->linesize[1] * uvy + uvx;
            break;
        case V:
            offset = frame->linesize[2] * uvy + uvx;
            break;
    }
    return offset;
}

std::string get_version(){
    std::ostringstream version;
    version << VERSION_VIS_MAJOR << "." << VERSION_VIS_MINOR;
    return version.str();
}

int ffmpeg2visframe(VisYUVFrame *dst, struct AVFrame *src) {
    int res;

    for(int y = 0; y < src->height; y++){
        for(int x = 0; x < src->width; x++){

            visBrush brush;

            brush.Y = (PixelValue)src->data[0][yuv_pixel_offset(src, x, y, Y)];
            brush.U = (PixelValue)src->data[1][yuv_pixel_offset(src, x, y, U)];
            brush.V = (PixelValue)src->data[2][yuv_pixel_offset(src, x, y, V)];
            if((res = YUVPixel_Draw(dst, &brush, x, y)) != 0){
                return res;
            }

        }
    }
    return 0;
}


int make_pgm(const std::string &source, const std::string &output_file){
    Visualizer visualizer;
    visualizer.load(source);
    visualizer.init_video();
    visualizer.process();
    visualizer.rasterize();
    visImage *img = visualizer.get_image();
    image_save_pgm(img, output_file.c_str());

    return 0;
}

void init(){
    std::cerr << "Im initing from " __FILE__;
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
    av_register_all();
#endif
}

PYBIND11_MODULE(visvid, m){
    m.def("get_version", &get_version);
    m.def("initialize", &init),
    m.def("make_pgm", &make_pgm, py::arg("source"), py::arg("output"));
}