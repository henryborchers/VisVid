//
// Created by henry on 4/24/19.
//

#include "Visualizer.h"
#include "visvid_exceptions.h"

extern "C"{
#include "shared/decode.h"
#include <libavutil/pixdesc.h>
#include <visvid/utils.h>
}
#include <iostream>
#include <memory>
#include <stdexcept>

const int MAX_BUFFER_SIZE = 200;


enum class pixel_component{
    Y = 1,
    U = 2,
    V = 3,
};



size_t yuv_pixel_offset(const AVFrame *frame, int x, int y, enum pixel_component component){
    size_t offset = 0;
    const AVPixFmtDescriptor * desc = av_pix_fmt_desc_get((AVPixelFormat)frame->format);
    signed int uvx = x >> desc->log2_chroma_w;
    signed int uvy = y >> desc->log2_chroma_h;

    switch(component){
        case pixel_component::Y:
            offset = frame->linesize[0] * y + x;
            break;
        case pixel_component::U:
            offset = frame->linesize[1] * uvy + uvx;
            break;
        case pixel_component::V:
            offset = frame->linesize[2] * uvy + uvx;
            break;
        default:
            break;
    }
    return offset;
}



void Visualizer::load(const std::string &source) {
    mSource = source;
}

void Visualizer::process() const{
    int ret;
    AVPacket pkt;

    // This method uses exceptions, so this should be placed somewhere else so it can be cleaned up if
    // ever fails and an exception is thrown.
    // perhaps moving frame variable a private member variable
    AVFrame *frame = av_frame_alloc();
    if(frame == nullptr){
        throw PyVisVidException("Could not allocate a video frame");
    }
    while(true) {
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

            ret = decode(mCodecCtx.get(), frame, &pkt);
            if(ret == AVERROR(EAGAIN)) {
                continue;
            }
            if(ret < 0){
                char error_msg[1000];
                av_strerror(ret, error_msg, 1000);
                puts(error_msg);
            }
            else if (ret == 1){

                process_frame(frame);

            }

        }
        av_packet_unref(&pkt);
        if(mCodecCtx->frame_number >= MAX_BUFFER_SIZE){
            break;
        }

    }
    av_frame_free(&frame);
}

void Visualizer::process_frame(const AVFrame *frame) const {
    std::shared_ptr<VisYUVFrame> yuvFrame(VisYUVFrame_Create(), [](VisYUVFrame *p){VisYUVFrame_Destroy(&p);});
    if(yuvFrame == nullptr){
        throw PyVisVidException("VisYUVFrame_Create failed\n");
    }
    VisYUVFrame_SetSize(yuvFrame.get(), frame->width, frame->height);
    if(ffmpeg2visframe(yuvFrame.get(), frame) !=0){
        throw PyVisVidException("ffmpeg2visframe failed\n");
    }
    int frame_width = mAvFormatCtx->streams[mVideoStream]->codecpar->width;

    visProcessContext proCtx;
    proCtx.processCb = visVisResult_CaculateBrightestOverWidth;
    visVisualResult     result;
    process_frame_result(yuvFrame.get(), frame_width, proCtx, result);

    VisVisualResult_Cleanup(&result);
}

void Visualizer::process_frame_result(const VisYUVFrame *yuvFrame, int frame_width, const visProcessContext &proCtx,
                                      visVisualResult &result) const {
    if(VisVisualResult_Init(&result) != 0 ){
        throw PyVisVidException("Unable to initialize a visVisualResult");
    }

    auto slice = std::make_unique<PixelValue[]>(frame_width);
    if(VisVisualResult_SetSize(&result, frame_width) != 0){
        throw PyVisVidException("VisVisualResult_SetSize failed \n");
    }
//                FIXME:!!!
    if(visVisProcess(&result, yuvFrame, &proCtx, slice.get()) != 0){
        throw PyVisVidException("visVisProcess failed");
    }
    if(mCodecCtx->frame_number % 100 == 0){
        std::cout << "Adding frame " << mCodecCtx->frame_number << "to buffer\n";
    }
    if(mCodecCtx->frame_number >= MAX_BUFFER_SIZE){
        return;
    }
    if(visBuffer_PushBackResult(mBuffer.get(), &result) != 0){
        throw PyVisVidException("visBuffer_PushBackResult failed");
    }
}

visImage *Visualizer::get_image() {
    return &mImage;
}

void Visualizer::rasterize() {
    mView = VisView_Create(mBuffer->bufferWidth, mCodecCtx->frame_number);

    if(visView_Update4(mView, mBuffer.get()) != 0){
        throw PyVisVidException("visView_Update4 failed");
    }
    visImage_Alloc(&mImage, mView->width, mView->height, 1);
    if(visView_GenerateBW(&mImage, mView) != 0){
        throw PyVisVidException("visView_GenerateBW");

    }



}

void Visualizer::init_video() {
    AVFormatContext *ctx = nullptr;

    if(avformat_open_input(&ctx, mSource.c_str(), nullptr, nullptr)) {
        throw PyVisVidException("unable to open file");
    }
    mAvFormatCtx = std::shared_ptr<AVFormatContext>(ctx, [](AVFormatContext *p){
        if(p != nullptr){
            avformat_close_input(&p);
        }

    });
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
    mCodecCtx = std::shared_ptr<AVCodecContext>(avcodec_alloc_context3(codec), [](AVCodecContext *p){
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
    mBuffer = std::shared_ptr<visBuffer>(VisBuffer_Create2(frame_width, MAX_BUFFER_SIZE), [](visBuffer *p){
        VisBuffer_Destroy(&p);
    });
    if(!mBuffer){
        throw PyVisVidException("Unable to allocate a visbuffer\n");

    }

}

int Visualizer::ffmpeg2visframe(VisYUVFrame *dst, const struct AVFrame *src) {

        int res;

        for(int y = 0; y < src->height; y++){
            for(int x = 0; x < src->width; x++){

                visBrush brush;

                brush.Y = src->data[0][yuv_pixel_offset(src, x, y, pixel_component::Y)];
                brush.U = src->data[1][yuv_pixel_offset(src, x, y, pixel_component::U)];
                brush.V = src->data[2][yuv_pixel_offset(src, x, y, pixel_component::V)];
                if((res = YUVPixel_Draw(dst, &brush, x, y)) != 0){
                    return res;
                }

            }
        }
        return 0;


}

