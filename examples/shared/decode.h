//
// Created by henry on 4/12/19.
//

#ifndef LIBVISVID_DECODE_H
#define LIBVISVID_DECODE_H
// TODO: check if c or c++ and if c++ use extern c
#include <libavcodec/avcodec.h>

int decode(AVCodecContext *dec_cts, AVFrame *frame, AVPacket *ptk);

#endif //LIBVISVID_DECODE_H
