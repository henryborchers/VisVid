//
// Created by henry on 4/24/19.
//


#include "Visualizer.h"
extern "C"{
#include "shared/decode.h"
#include <libavutil/pixdesc.h>
#include <visvid/utils.h>

}
#include <iostream>
#include <stdexcept>
#define MAX_BUFFER_SIZE 200


enum pixel_component{
    Y,
    U,
    V,
};

size_t yuv_pixel_offset(AVFrame *frame, int x, int y, enum pixel_component component);

size_t yuv_pixel_offset(AVFrame *frame, int x, int y, enum pixel_component component){
    size_t offset = 0;
    auto *desc = (AVPixFmtDescriptor *) av_pix_fmt_desc_get((AVPixelFormat)frame->format);
    signed int uvx = x >> desc->log2_chroma_w;
    signed int uvy = y >> desc->log2_chroma_h;

    switch(component){
        case Y:
            offset = frame->linesize[0] * y + x;
            break;
        case U:
            offset = frame->linesize[1] * uvy + uvx;
            break;
        case V:
            offset = frame->linesize[2] * uvy + uvx;
            break;
    }
    return offset;
}
Visualizer::Visualizer()
    : mVideoStream(-1){
    AVFrame *frame = av_frame_alloc();
    if(frame == nullptr){
        throw std::runtime_error("Could not allocate a video frame\n");
    }
}

Visualizer::~Visualizer() {
    if(mAvFormatCtx != nullptr){
        avformat_close_input(&mAvFormatCtx);
    }

    if(mCodecCtx != nullptr) {
        avcodec_free_context(&mCodecCtx);
    }
    VisBuffer_Destroy(&mBuffer);

}

void Visualizer::load(const std::string &source) {
    mSource = source;
}

void Visualizer::process() {
    int ret;
    AVPacket pkt;

    // This method uses exceptions, so this should be placed somewhere else so it can be cleaned up if
    // ever fails and an exception is thrown.
    // perhaps moving frame variable a private member variable
    AVFrame *frame = av_frame_alloc();
    if(!frame){
        throw std::runtime_error("Could not allocate a video frame");
    }
    while(1) {
        if((ret = av_read_frame(mAvFormatCtx, &pkt)) < 0){
            if(ret == AVERROR_EOF){
                ret = 0;
                break;
            }
            char error_msg[1000];
            av_strerror(ret, error_msg, 1000);
            puts(error_msg);
        }
        if(pkt.stream_index == mVideoStream){

            ret = decode(mCodecCtx, frame, &pkt);
            if(ret == AVERROR(EAGAIN)) {
                continue;
            }
            if(ret < 0){
                char error_msg[1000];
                av_strerror(ret, error_msg, 1000);
                puts(error_msg);
            }
            else if (ret == 1){

                VisYUVFrame *yuvFrame = VisYUVFrame_Create();
                if(yuvFrame == nullptr){
                    throw std::runtime_error("VisYUVFrame_Create failed\n");
                }
                VisYUVFrame_SetSize(yuvFrame, frame->width, frame->height);
                if((ret = ffmpeg2visframe(yuvFrame, frame)) !=0){
                    throw std::runtime_error("ffmpeg2visframe failed\n");
                }

                visProcessContext proCtx;
                proCtx.processCb = visVisResult_CaculateBrightestOverWidth;
                visVisualResult     result;
                if((ret = VisVisualResult_Init(&result)) != 0 ){
                    throw std::runtime_error("Unable to initialize a visVisualResult");
                }
                int frame_width = mAvFormatCtx->streams[mVideoStream]->codecpar->width;
                if((ret = VisVisualResult_SetSize(&result, frame_width)) != 0){
                    throw std::runtime_error("VisVisualResult_SetSize failed \n");
                }
//                FIXME:!!!
                PixelValue *slice= new PixelValue[frame_width];
                if((ret = visVisProcess(&result, yuvFrame, &proCtx, slice)) != 0){
                    throw std::runtime_error("visVisProcess failed");
                }
                if(mCodecCtx->frame_number % 100 == 0){
                    std::cout << "Adding frame " << mCodecCtx->frame_number << "to buffer\n";
                }
                if(mCodecCtx->frame_number >= MAX_BUFFER_SIZE){
                    break;
                }
                if((ret = visBuffer_PushBackResult(mBuffer, &result)) != 0){
                    throw std::runtime_error("visBuffer_PushBackResult failed");
                }
                delete[] slice;
                VisVisualResult_Cleanup(&result);
                VisYUVFrame_Destroy(&yuvFrame);
            }

        }
        av_packet_unref(&pkt);
        if(mCodecCtx->frame_number >= MAX_BUFFER_SIZE){
            break;
        }

    }
    av_frame_free(&frame);
}

visImage *Visualizer::get_image() {
    int ret;
//    if((ret = visView_GenerateBW(&mImage, mView)) != 0){
//        fprintf(stderr, "visView_GenerateBW failed with value %d\n",  ret);
//
//    }
    return &mImage;
}

void Visualizer::rasterize() {
    int ret;
    mView = VisView_Create(mBuffer->bufferWidth, mCodecCtx->frame_number);

    if((ret = visView_Update4(mView, mBuffer)) != 0){
        throw std::runtime_error("visView_Update4 failed");
    }
    visImage_Alloc(&mImage, mView->width, mView->height, 1);
    if((ret = visView_GenerateBW(&mImage, mView)) != 0){
        throw std::runtime_error("visView_GenerateBW");

    }



}

void Visualizer::init_video() {
    int ret = 0;
    if(( ret = avformat_open_input(&mAvFormatCtx, mSource.c_str(), nullptr, nullptr))){
        throw std::runtime_error("unable to open file");
    }

    for (unsigned int stream_number = 0; stream_number < mAvFormatCtx->nb_streams; stream_number++) {
        if(mAvFormatCtx->streams[stream_number]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            mVideoStream = stream_number;
            break;
        }
    }

    const AVCodec *codec = avcodec_find_decoder(mAvFormatCtx->streams[mVideoStream]->codecpar->codec_id);
    if(!codec){
        throw std::runtime_error("unable to find codec");
    }

    mCodecCtx =  avcodec_alloc_context3(codec);
    if(!mCodecCtx){
        throw std::runtime_error("Could not allocate video codec context");
    }

    ret = avcodec_parameters_to_context(mCodecCtx, mAvFormatCtx->streams[mVideoStream]->codecpar);
    if(ret < 0){
        throw std::runtime_error("Could not set codec context parameters\n");
    }
    if(avcodec_open2(mCodecCtx, codec, nullptr) < 0){
        throw std::runtime_error("Could Not open codec\n");
    }
    int frame_width = mAvFormatCtx->streams[mVideoStream]->codecpar->width;
    mBuffer = VisBuffer_Create2(frame_width, MAX_BUFFER_SIZE);

    if(mBuffer == nullptr){
        throw std::runtime_error("Unable to allocate a visbuffer\n");
    }



}

int Visualizer::ffmpeg2visframe(VisYUVFrame *dst, struct AVFrame *src) {

        int res;

        for(int y = 0; y < src->height; y++){
            for(int x = 0; x < src->width; x++){

                visBrush brush;

                brush.Y = (PixelValue)src->data[0][yuv_pixel_offset(src, x, y, Y)];
                brush.U = (PixelValue)src->data[1][yuv_pixel_offset(src, x, y, U)];
                brush.V = (PixelValue)src->data[2][yuv_pixel_offset(src, x, y, V)];
                if((res = YUVPixel_Draw(dst, &brush, x, y)) != 0){
                    return res;
                }

            }
        }
        return 0;


}

