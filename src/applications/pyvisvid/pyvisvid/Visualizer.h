//
// Created by henry on 4/24/19.
//

#ifndef LIBVISVID_VISUALIZER_H
#define LIBVISVID_VISUALIZER_H

#include <string>

extern "C"{
#include <libavformat/avformat.h>
#include <visvid/visvid.h>
};

class Visualizer {
    std::string mSource;
    AVFormatContext *mAvFormatCtx;
    int mVideoStream;
    visBuffer *mBuffer;
    AVCodecContext *mCodecCtx;
    static int ffmpeg2visframe(VisYUVFrame *dst, struct AVFrame *src);
    visImage mImage;
    visView *mView;

public:
    Visualizer();

    void load(const std::string &source);
    void init_video();
    void process();
    void rasterize();
    visImage *get_image();

    virtual ~Visualizer();
};


#endif //LIBVISVID_VISUALIZER_H
