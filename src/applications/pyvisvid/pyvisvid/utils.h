//
// Created by Borchers, Henry Samuel on 2/26/21.
//

#ifndef LIBVISVID_UTILS_H
#define LIBVISVID_UTILS_H
#include <string>
extern "C"{
#include <libavutil/frame.h>
};
enum pixel_component{
    Y,
    U,
    V,
};
size_t yuv_pixel_offset(const AVFrame *frame, int x, int y, enum pixel_component component);
std::string get_version();
#endif //LIBVISVID_UTILS_H
