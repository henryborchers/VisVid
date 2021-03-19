//
// generate.cpp
// Created by Borchers, Henry Samuel on 2/26/21.
//

#include "generate.h"
#include "AbsPgmStrategy.h"
#include "Visualizer.h"
#include "visvid_exceptions.h"
#include "utils.h"
extern "C" {
#include "visvid/utils.h"
#include "shared/pgm.h"
#include <libavutil/pixdesc.h>
}
#include <iostream>

class StandardPgmStrategy : public AbsPgmStrategy {
public:
    int make_pgm(const std::string &source, const std::string &output_file) const override;
};

int StandardPgmStrategy::make_pgm(const std::string &source, const std::string &output_file) const{
    Visualizer visualizer;
    visualizer.load(source);
    visualizer.init_video();
    visualizer.process();
    visualizer.rasterize();
    const visImage *img = visualizer.get_image();
    image_save_pgm(img, output_file.c_str());
    return 0;
}

class SplitPgmStrategy : public AbsPgmStrategy {
public:
    int make_pgm(const std::string &source, const std::string &output_file) const override;
};

int SplitPgmStrategy::make_pgm(const std::string &source, const std::string &output_file) const {
    Processor processor(std::make_shared<VideoFile>(source));
    const std::shared_ptr<visImage> img = processor.process();
    image_save_pgm(img.get(), output_file.c_str());
    return 0;
}

int make_pgm(const std::string &source, const std::string &output_file){
    const SplitPgmStrategy strategy;
    return strategy.make_pgm(source, output_file);
}

//int make_pgm(const std::string &source, const std::string &output_file, std::shared_ptr<AbsPgmStrategy> strategy){
//    return strategy->make_pgm(source, output_file);
//}
VideoFile::VideoFile(const std::string &source): mSource(source) {}

const std::string &VideoFile::getSource() const {
    return mSource;
}

Processor::Processor(std::shared_ptr<VideoFile> videoFile): mVideoFile(videoFile) {

}
const int MAX_BUFFER_SIZE = 200;

std::shared_ptr<visImage> Processor::process() {

    AVFormatContext *ctx = nullptr;
    if(avformat_open_input(&ctx, mVideoFile->getSource().c_str(), nullptr, nullptr)<0) {
        throw PyVisVidException("unable to open file");
    }
    std::shared_ptr<AVFormatContext> mAvFormatCtx = std::shared_ptr<AVFormatContext>(ctx, [](AVFormatContext *p){
        if(p != nullptr){
            avformat_close_input(&p);
        }
    });

    int mVideoStream = 0;
    for (unsigned int stream_number = 0; stream_number < mAvFormatCtx->nb_streams; stream_number++) {
        if(mAvFormatCtx->streams[stream_number]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            mVideoStream = stream_number;
            break;
        }
    }

    const AVCodec *codec = avcodec_find_decoder(mAvFormatCtx->streams[mVideoStream]->codecpar->codec_id);
    if(codec == nullptr){
        throw PyVisVidException("unable to find codec");
    }
    auto mCodecCtx = std::shared_ptr<AVCodecContext>(avcodec_alloc_context3(codec), [](AVCodecContext *p){
        if(p != nullptr){
            avcodec_free_context(&p);
        }
    });

    if(!mCodecCtx){
        throw PyVisVidException("Could not allocate video codec context");
    }

    if(avcodec_parameters_to_context(mCodecCtx.get(), mAvFormatCtx->streams[mVideoStream]->codecpar) < 0){
        throw PyVisVidException("Could not set codec context parameters\n");
    }
    if(avcodec_open2(mCodecCtx.get(), codec, nullptr) < 0){
        throw PyVisVidException("Could Not open codec\n");
    }
    int frame_width = mAvFormatCtx->streams[mVideoStream]->codecpar->width;
    auto buffer = std::shared_ptr<visBuffer>(VisBuffer_Create2(frame_width, MAX_BUFFER_SIZE), [](visBuffer *p){
        VisBuffer_Destroy(&p);
    });
    if(!buffer){
        throw PyVisVidException("Unable to allocate a visbuffer\n");

    }


//    ===============================================================

    AVPacket pkt;
    std::shared_ptr<AVFrame> frame(av_frame_alloc(), [](AVFrame *p){av_frame_free(&p);});
    if(!frame){
        throw PyVisVidException("Could not allocate a video frame");
    }
    while(true) {
        int ret;
        if((ret = av_read_frame(mAvFormatCtx.get(), &pkt)) < 0){
            if(ret == AVERROR_EOF){
                ret = 0;
                break;
            }
            char error_msg[1000];
            av_strerror(ret, error_msg, 1000);
            puts(error_msg);
        }
        if(pkt.stream_index == mVideoStream){

            ret = decode(mCodecCtx, frame, pkt);
            if(ret == AVERROR(EAGAIN)) {
                continue;
            }
            if(ret < 0){
                char error_msg[1000];
                av_strerror(ret, error_msg, 1000);
                puts(error_msg);
            }
            else if (ret == 1){

                process_frame(frame, frame_width, mCodecCtx, buffer);

            }

        }
        av_packet_unref(&pkt);
        if(mCodecCtx->frame_number >= MAX_BUFFER_SIZE){
            break;
        }
    }

//    ===============================================================
    std::shared_ptr<visView> mView(VisView_Create(buffer->bufferWidth, mCodecCtx->frame_number), [](visView *ptr){VisView_Destroy(&ptr);});
    if(visView_Update4(mView.get(), buffer.get()) != 0){
        throw PyVisVidException("visView_Update4 failed");
    }
    std::shared_ptr<visImage> img = this->generateImage(mView);


    return img;
}
int Processor::decode(std::shared_ptr<AVCodecContext> codecCtx, std::shared_ptr<AVFrame> frame, AVPacket &packet) {
//TODO: fix up here
    int ret;

    ret = avcodec_send_packet(codecCtx.get(), &packet);
    if(ret < 0 ){
        char error_msg[1000];
        av_strerror(ret,error_msg, 1000);
        fprintf(stderr, "error sending a packet for decoding. Reason: %s\n", error_msg);
        return ret;
    }
//    while (ret >=0){
    ret = avcodec_receive_frame(codecCtx.get(), frame.get());
    if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
        return ret;
    } else if (ret < 0){
        fprintf(stderr, "error During decoding\n");
        return ret;
    }


    return 1;
//    }
//    char error_msg[1000];
//    av_strerror(ret,error_msg, 1000);
//    fprintf(stderr, "error . Reason: %s\n", error_msg);
//    exit(1);
}

void Processor::process_frame(std::shared_ptr<AVFrame> frame, int frame_width, std::shared_ptr<AVCodecContext> codecContext, std::shared_ptr<visBuffer> buffer) {
    std::shared_ptr<VisYUVFrame> yuvFrame(VisYUVFrame_Create(), [](VisYUVFrame *p){VisYUVFrame_Destroy(&p);});
    if(yuvFrame == nullptr){
        throw PyVisVidException("VisYUVFrame_Create failed\n");
    }
    VisYUVFrame_SetSize(yuvFrame.get(), frame->width, frame->height);
    if(this->ffmpeg2visframe(yuvFrame, frame) != 0){
        throw PyVisVidException("ffmpeg2visframe failed\n");
    }

    visProcessContext proCtx;
    proCtx.processCb = visVisResult_CaculateBrightestOverWidth;

    auto *result = (visVisualResult*)malloc(sizeof(visVisualResult));
    if(VisVisualResult_Init(result) != 0 ){
        throw PyVisVidException("Unable to initialize a visVisualResult");
    }
    std::shared_ptr<visVisualResult> r(result, [](visVisualResult* ptr){VisVisualResult_Destroy(&ptr);});

    process_frame_result(yuvFrame, frame_width, proCtx, r, codecContext, buffer);
}



int Processor::ffmpeg2visframe(std::shared_ptr<VisYUVFrame> dst, std::shared_ptr<AVFrame> src) {
    Visualizer::ffmpeg2visframe(dst.get(), src.get());
    return 0;
}

void Processor::process_frame_result(std::shared_ptr<VisYUVFrame> yuvFrame, int frame_width, const visProcessContext &proCtx,
                                     std::shared_ptr<visVisualResult> result, std::shared_ptr<AVCodecContext> codecContext,
                                     std::shared_ptr<visBuffer> buffer) {


        auto slice = std::make_unique<PixelValue[]>(frame_width);
        if(VisVisualResult_SetSize(result.get(), frame_width) != 0){
            throw PyVisVidException("VisVisualResult_SetSize failed \n");
        }
        if(visVisProcess(result.get(), yuvFrame.get(), &proCtx, slice.get()) != 0){
            throw PyVisVidException("visVisProcess failed");
        }
        if(codecContext->frame_number % 100 == 0){
            std::cout << "Adding frame " << codecContext->frame_number << "to buffer\n";
        }
        if(codecContext->frame_number >= MAX_BUFFER_SIZE){
            return;
        }
        if(visBuffer_PushBackResult(buffer.get(), result.get()) != 0){
            throw PyVisVidException("visBuffer_PushBackResult failed");
        }

}

std::shared_ptr<visImage> Processor::generateImage(std::shared_ptr<visView> mView) {
    std::shared_ptr<visImage> img(
            new visImage,
            [](visImage *ptr){
                visImage_FreeData(ptr);
                delete(ptr);
            }
    );
    visImage_Alloc(img.get(), mView->width, mView->height, 1);
    if(visView_GenerateBW(img.get(), mView.get()) != 0){
        throw PyVisVidException("visView_GenerateBW");

    }
    return img;
}
