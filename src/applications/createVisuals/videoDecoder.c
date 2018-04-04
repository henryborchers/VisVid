//
// Created by Borchers, Henry Samuel on 11/25/16.
//

#include "videoDecoder.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/version.h>
#include <libavutil/avutil.h>
#include <libavutil/log.h>
#include <libavutil/pixfmt.h>
#include <libavutil/version.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <swscale.h>
#include <stdbool.h>
#include <unistd.h>
#include "error.h"
#include "mem.h"
//#include <swscale.h>

#define FILE_PATH_MAX 10000



struct DecoderContext{
    AVCodecContext      *codecContext;
    AVFormatContext     *formatContext;
    AVFrame             *frame;
    struct SwsContext   *sws_ctx;
    char                *filename;
    int                 video_stream_idx;
    AVPacket            pkt;
};

static bool fileExists(const char *filename);
static int decode(AVCodecContext *pContext, AVFrame *pFrame, int *got_frame, AVPacket *pkt);

void decoder_init() {
    av_register_all();
}

DecoderContext *decoderContext_Create(const char *filename) {
    int             ret;
    size_t          file_name_len = 0;
    DecoderContext  *tmp = NULL;
    AVCodec         *decoder = NULL;


    // calculate the length of file name
    file_name_len = strnlen(filename, FILE_PATH_MAX);
    if(file_name_len < 1){
        return NULL;
    }

    // Check if filename exists.
    if(!fileExists(filename)){
        return NULL;
    };

    tmp = (DecoderContext*)malloc(sizeof(DecoderContext));
    if(tmp == NULL){
        return NULL;
    }

    tmp->filename           = NULL;
    tmp->formatContext      = NULL;
    tmp->frame              = NULL;
    tmp->codecContext       = NULL;
    tmp->video_stream_idx   = -1;

    tmp->filename = (char *)malloc(sizeof(char) * (file_name_len + 1));
    if(tmp->filename == NULL){
        free(tmp);
        return NULL;
    }

    strncpy(tmp->filename, filename, file_name_len);
    if((ret = avformat_open_input(&tmp->formatContext, filename, NULL, NULL)) < 0){
        char error_msg[1000];
        av_strerror(ret, error_msg, 1000);
        av_log(NULL, AV_LOG_ERROR, "Cannot open input: %s\n", error_msg);
        return NULL;
    };

    if((ret = avformat_find_stream_info(tmp->formatContext, NULL)) < 0){
        char error_msg[1000];
        av_strerror(ret, error_msg, 1000);
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream: %s\n", error_msg);
        return NULL;
    }

    ret = av_find_best_stream(tmp->formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder,0);
    if(ret < 0){
        char error_msg[1000];
        av_strerror(ret, error_msg, 1000);
        av_log(NULL, AV_LOG_ERROR, "Cannot find video stream: %s\n", error_msg);
        return NULL;
    } else {
        tmp->video_stream_idx = ret;
    }

    tmp->codecContext = avcodec_alloc_context3(decoder);
    if(tmp->codecContext == NULL){

        av_log(NULL, AV_LOG_ERROR, "Unable to allocate codec context\n");
        return NULL;
    }
#if LIBAVFORMAT_VERSION_INT < AV_VERSION_INT(57,0,0)
    AVCodecContext *pCodecCtxOrg = NULL;
    pCodecCtxOrg = tmp->formatContext->streams[tmp->video_stream_idx]->codec;
    if((ret = avcodec_copy_context(tmp->codecContext, pCodecCtxOrg)) != 0){
        char error_msg[1000];
        av_strerror(ret, error_msg, 1000);
        av_log(NULL, AV_LOG_ERROR, "%s\n", error_msg);
    }
#else



    if((ret = avcodec_parameters_to_context(tmp->codecContext, tmp->formatContext->streams[tmp->video_stream_idx]->codecpar)) < 0){
        char error_msg[1000];
        av_strerror(ret, error_msg, 1000);
        av_log(NULL, AV_LOG_ERROR, "%s\n", error_msg);
        return NULL;
    }
#endif

    if((ret = avcodec_open2(tmp->codecContext, decoder, NULL)) < 0){
        char error_msg[1000];
        av_strerror(ret, error_msg, 1000);
        av_log(NULL, AV_LOG_ERROR, "Cannot open video decoder %s\n", error_msg);
        return NULL;
    }

    tmp->frame = av_frame_alloc();
    if(tmp->frame == NULL){
        av_log(NULL, AV_LOG_ERROR, "Unable to allocate a frame\n");
        return NULL;
    }

    av_init_packet(&tmp->pkt);

    tmp->sws_ctx = sws_getContext(tmp->codecContext->width,
                                  tmp->codecContext->height,
                                  tmp->codecContext->pix_fmt,
                                  tmp->codecContext->width,
                                  tmp->codecContext->height,
                                  AV_PIX_FMT_YUV420P,
                                  SWS_BILINEAR, NULL, NULL, NULL);

    return tmp;
}

void decoderContext_Destroy(DecoderContext **pDecoderContext) {
    av_free((*pDecoderContext)->frame);
    avcodec_close((*pDecoderContext)->codecContext);
    avcodec_free_context(&(*pDecoderContext)->codecContext);
    avformat_close_input(&(*pDecoderContext)->formatContext);
    free((*pDecoderContext)->filename);
    (*pDecoderContext)->video_stream_idx = -1;
    free(*pDecoderContext);
    *pDecoderContext = NULL;
}

int decoderContext_NextFrame(DecoderContext *pDecoderContext, AVFrame **out) {
    int ret;
    int got_frame;
    *out = NULL;
    pDecoderContext->pkt.data = NULL;
    pDecoderContext->pkt.size = 0;

    while(av_read_frame(pDecoderContext->formatContext, &pDecoderContext->pkt) >=0){
        if(pDecoderContext->pkt.stream_index == pDecoderContext->video_stream_idx){
            ret = decode(pDecoderContext->codecContext, pDecoderContext->frame, &got_frame, &pDecoderContext->pkt);
            if(ret < 0){
                av_log(NULL, AV_LOG_ERROR, "Error with a frame %s\n",av_err2str(ret));
                return ret;
            }

            if(got_frame){
                AVFrame pict;
                pict.data[0] = pDecoderContext->frame->data[0];
                pict.data[1] = pDecoderContext->frame->data[1];
                pict.data[2] = pDecoderContext->frame->data[2];

                pict.linesize[0] = pDecoderContext->frame->linesize[0];
                pict.linesize[1] = pDecoderContext->frame->linesize[1];
                pict.linesize[2] = pDecoderContext->frame->linesize[2];

                if((ret = sws_scale(pDecoderContext->sws_ctx,
                          (uint8_t const * const *)pDecoderContext->frame->data,
                          pDecoderContext->frame->linesize, 0,
                          pDecoderContext->codecContext->height,
                          pict.data, pict.linesize)) < 0){
                    *out = NULL;
                    fprintf(stderr, "sws_scale failed\n");
                    return ret;
                };
                *out = pDecoderContext->frame;
                return 0;

            }
        }
    }
    return AVERROR_EOF;
}

void decoderContext_GetSize(DecoderContext *pDecoderContext, int *width, int *height) {
    *width = pDecoderContext->codecContext->width;
    *height = pDecoderContext->codecContext->height;

}

int decoderContext_Rewind(DecoderContext *pDecoderContext) {
//    FIXME: av_seek_frame should point to the first packet position which isn't always going to be 0.
    int ret;

    if((ret = av_seek_frame(pDecoderContext->formatContext, pDecoderContext->video_stream_idx, 0, AVSEEK_FLAG_BACKWARD)) != 0){
        fprintf(stderr, "Failed to rewind");
        return ret;
    };
    return 0;
}


bool fileExists(const char *filename) {
    if(access(filename, F_OK) == -1){
        return false;
    }
    return true;
}


int decode(AVCodecContext *pContext, AVFrame *pFrame, int *got_frame, AVPacket *pkt) {
#if LIBAVFORMAT_VERSION_INT < AV_VERSION_INT(57,0,0)
    return avcodec_decode_video2(pContext, pFrame, got_frame, pkt);
#else
    int ret;

    *got_frame = 0;

    if(pkt) {
        ret = avcodec_send_packet(pContext, pkt);
        if(ret < 0){
            return ret == AVERROR_EOF ? 0 : ret;
        }
    }

    ret = avcodec_receive_frame(pContext, pFrame);
    if(ret < 0 && ret != AVERROR(EAGAIN) && ret !=AVERROR_EOF) {
        return ret;
    } if (ret >= 0){
        *got_frame = 1;
    }

    return 0;
#endif
}