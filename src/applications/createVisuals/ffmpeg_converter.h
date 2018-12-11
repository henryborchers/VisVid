//
// Created by Borchers, Henry Samuel on 12/17/16.
//
#ifndef LIBVISVID_FFMPEG_CONVERTER_H
#define LIBVISVID_FFMPEG_CONVERTER_H
//#include <libavutil/frame.h>
//#include <visFrame.h>
#include "visvid/visvid.h"
struct AVFrame;



int ffmpeg2visframe(VisYUVFrame *dst, struct AVFrame *src);

#endif //LIBVISVID_FFMPEG_CONVERTER_H
