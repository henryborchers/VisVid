//
// Created by Borchers, Henry Samuel on 2/26/21.
//

#ifndef LIBVISVID_GENERATE_H
#define LIBVISVID_GENERATE_H
#include <string>
#include "AbsPgmStrategy.h"
#include "Visualizer.h"

#include <optional>
#include <memory>
int make_pgm(const std::string &source, const std::string &output_file);
//int make_pgm(const std::string &source, const std::string &output_file, const AbsPgmStrategy &strategy);

class VideoFile{
    std::string mSource;
public:
    const std::string &getSource() const;

public:
    explicit VideoFile(const std::string &source);
};


class VideoFilePlayer: public VideoFile{
public:

    void forward();
    void back();
    VideoFilePlayer(const std::string &source);
};

class Processor{
private:
    enum class pixel_component{
        Y = 1,
        U = 2,
        V = 3,
    };
    std::shared_ptr<VideoFile> mVideoFile;
//    std::shared_ptr<AVFormatContext> open(const std::string &filename);
    static std::shared_ptr<AVCodecContext> getCodecContext(const std::shared_ptr<AVFormatContext> &mAvFormatCtx, const AVCodec *codec, int mVideoStream);
    static std::shared_ptr<AVFrame> allocateEmptyFrame();
    static std::shared_ptr<visBuffer> createVisvidBuffer(int frame_width, int buffer_size);
    static std::shared_ptr<AVFormatContext> createFormatContext(const std::string &filename);
public:
    explicit Processor(std::shared_ptr<VideoFile> videoFile);
    virtual std::shared_ptr<visImage> process();

    static int decode(std::shared_ptr<AVCodecContext> codecCtx, std::shared_ptr<AVFrame> frame, AVPacket &packet);

    static void process_frame(std::shared_ptr<AVFrame> frame, int frame_width, std::shared_ptr<AVCodecContext> codecContext, std::shared_ptr<visBuffer> buffer);

    static int ffmpeg2visframe(std::shared_ptr<VisYUVFrame> dst, std::shared_ptr<AVFrame> src);
    static void process_frame_result(std::shared_ptr<VisYUVFrame> yuvFrame, int frame_width, const visProcessContext &proCtx,
                              std::shared_ptr<visVisualResult> result, std::shared_ptr<AVCodecContext> codecContext, std::shared_ptr<visBuffer> buffer);

    static std::shared_ptr<visImage> generateImage(std::shared_ptr<visView> sharedPtr);

    static int getVideoStream(const std::shared_ptr<AVFormatContext> &mAvFormatCtx);
    static std::optional<unsigned int> getVideoStream1(const std::shared_ptr<AVFormatContext> &formatContext);



};

class Processor2: public Processor{
public:
    Processor2(const std::shared_ptr<VideoFile> &videoFile);


};

#endif //LIBVISVID_GENERATE_H
