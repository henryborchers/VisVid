//
// Created by henry on 4/24/19.
//

#ifndef LIBVISVID_VISUALIZER_H
#define LIBVISVID_VISUALIZER_H

#include <string>
#include <memory>
extern "C"{
#include <libavformat/avformat.h>
#include <visvid/visvid.h>
};

class Visualizer {
    std::string mSource;
    std::shared_ptr<AVFormatContext> mAvFormatCtx;
    int mVideoStream = -1;
    std::shared_ptr<visBuffer> mBuffer;
    std::shared_ptr<AVCodecContext> mCodecCtx;
    static int ffmpeg2visframe(VisYUVFrame *dst, const struct AVFrame *src);
    visImage mImage;
    visView *mView;
    void process_frame(AVFrame *frame) const;
    void process_frame_result(const VisYUVFrame *yuvFrame, int frame_width, const visProcessContext &proCtx,
                              visVisualResult &result) const;
public:

    void load(const std::string &source);
    void init_video();
    void process() const;
    void rasterize();
    visImage *get_image();

};


#endif //LIBVISVID_VISUALIZER_H
