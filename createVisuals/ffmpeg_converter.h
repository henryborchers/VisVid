//
// Created by Borchers, Henry Samuel on 12/17/16.
//

#include <libavutil/frame.h>
#include <visFrame.h>

#ifndef LIBVISVID_FFMPEG_CONVERTER_H
#define LIBVISVID_FFMPEG_CONVERTER_H

int ffmpeg2visframe(VisYUVFrame *dst, struct AVFrame *src);

#endif //LIBVISVID_FFMPEG_CONVERTER_H
