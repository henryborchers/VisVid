//
// Created by Borchers, Henry Samuel on 2/26/21.
//

#ifndef LIBVISVID_GENERATE_H
#define LIBVISVID_GENERATE_H
#include <string>
#include "AbsPgmStrategy.h"
#include <memory>

#include "Visualizer.h"

int make_pgm(const std::string &source, const std::string &output_file);
//int make_pgm(const std::string &source, const std::string &output_file, const AbsPgmStrategy &strategy);

class VideoFile{
    std::string mSource;
public:
    const std::string &getSource() const;

public:
    explicit VideoFile(const std::string &source);
};



class Processor{
private:
    enum class pixel_component{
        Y = 1,
        U = 2,
        V = 3,
    };
    std::shared_ptr<VideoFile> mVideoFile;
    static size_t yuv_pixel_offset(std::shared_ptr<AVFrame> frame, int x, int y, enum pixel_component component);
public:
    explicit  Processor(std::shared_ptr<VideoFile> videoFile);
    std::shared_ptr<visImage> process();

    int decode(std::shared_ptr<AVCodecContext> codecCtx, std::shared_ptr<AVFrame> frame, AVPacket &packet);

    void process_frame(std::shared_ptr<AVFrame> frame, int frame_width, std::shared_ptr<AVCodecContext> codecContext, std::shared_ptr<visBuffer> buffer);

    int ffmpeg2visframe(std::shared_ptr<VisYUVFrame> dst, std::shared_ptr<AVFrame> src);
    void process_frame_result(std::shared_ptr<VisYUVFrame> yuvFrame, int frame_width, const visProcessContext &proCtx,
                              std::shared_ptr<visVisualResult> result, std::shared_ptr<AVCodecContext> codecContext, std::shared_ptr<visBuffer> buffer);

//    void process_frame_result(VisYUVFrame *pFrame, int width, visProcessContext context, visVisualResult result);
};
#endif //LIBVISVID_GENERATE_H
