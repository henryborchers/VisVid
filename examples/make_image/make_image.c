//
// Created by henry on 3/3/19.
//
#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>

#include <visvid/visvid.h>
#include <visvid/utils.h>
#include "../shared/decode.h"
#include "../shared/pgm.h"
//int decode(AVCodecContext *dec_cts, AVFrame *frame, AVPacket *ptk);

const char outfilename[] = "output.pgm";
#define MAX_BUFFER_SIZE 200


size_t yuv_pixel_offset(AVFrame *frame, int x, int y, char component, const AVPixFmtDescriptor *pixFormat){
    size_t offset = 0;
    signed int uvx = x >> pixFormat->log2_chroma_w;
    signed int uvy = y >> pixFormat->log2_chroma_h;

    switch(component){
        case 'y':
            offset = frame->linesize[0] * y + x;
            break;
        case 'u':
            offset = frame->linesize[1] * uvy + uvx;
            break;
        case 'v':
            offset = frame->linesize[2] * uvy + uvx;
            break;
        default:
            fprintf(stderr, "Invalid component selected, should be only or y, u, v.");
            exit(1); // Don't know what that character is, should be only or y, u, v.
    }
    return offset;
}

int ffmpeg2visframe(VisYUVFrame *dst, struct AVFrame *src) {
    int res;

    AVPixFmtDescriptor *desc = (AVPixFmtDescriptor *) av_pix_fmt_desc_get(src->format);
    for(int y = 0; y < src->height; y++){
        for(int x = 0; x < src->width; x++){

            visBrush brush;
            brush.Y = (PixelValue)src->data[0][yuv_pixel_offset(src, x, y, 'y', desc)];
            brush.U = (PixelValue)src->data[1][yuv_pixel_offset(src, x, y, 'u', desc)];
            brush.V = (PixelValue)src->data[2][yuv_pixel_offset(src, x, y, 'v', desc)];
            if((res = YUVPixel_Draw(dst, &brush, x, y)) != 0){
                return res;
            }

        }
    }
    return 0;
}


int main(int argc, char *argv[]){
    const char *filename;
    const AVCodec *codec;
    AVFrame *frame;
    AVCodecContext *codecCtx = NULL;
    AVCodecParserContext *parser;
    AVFormatContext *avFormatCtx = NULL;
    int video_stream = -1;

    AVPacket pkt;
    int ret = 0;

    if(argc < 2){
        fprintf(stderr, "Usage %s <input>\n", argv[0]);
        exit(1);
    }

    filename = argv[1];
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
    av_register_all();
#endif

    if(( ret = avformat_open_input(&avFormatCtx, filename, NULL, NULL))){
        fprintf(stderr, "unable to open %s\n", filename);
        return ret;
    }
    for (unsigned int stream_number = 0; stream_number < avFormatCtx->nb_streams; stream_number++) {
        if(avFormatCtx->streams[stream_number]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream = stream_number;
            break;
        }
    }
    codec = avcodec_find_decoder(avFormatCtx->streams[video_stream]->codecpar->codec_id);

    if(!codec){
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }

    parser = av_parser_init(avFormatCtx->streams[video_stream]->codecpar->codec_id);
    if(!parser){
        fprintf(stderr, "Parser not found\n");
        exit(1);
    }

    codecCtx = avcodec_alloc_context3(codec);
    if(!codecCtx){
        fprintf(stderr,  "Could not allocate video codec context\n");
        exit(1);
    }
    ret = avcodec_parameters_to_context(codecCtx, avFormatCtx->streams[video_stream]->codecpar);
    if(ret < 0){
        fprintf(stderr,  "Could not set codec context parameters\n");
        exit(1);
    }

    if(avcodec_open2(codecCtx, codec, NULL) < 0){
        fprintf(stderr, "Could Not open codec\n");
        exit(1);
    }

    frame = av_frame_alloc();
    if(!frame){
        fprintf(stderr, "Could not allocate a video frame\n");
        exit(1);
    }
    printf("Using %s\n",filename);
    int frame_width = avFormatCtx->streams[video_stream]->codecpar->width;
    visBuffer *buffer = VisBuffer_Create2(frame_width, MAX_BUFFER_SIZE);


    if(buffer == NULL){
        fprintf(stderr, "Unable to allocate a visbuffer\n");
        exit(1);
    }

    PixelValue *sliceBuffer = malloc(sizeof(PixelValue) * frame_width);
    while(1) {
        if((ret = av_read_frame(avFormatCtx, &pkt)) < 0){
            if(ret == AVERROR_EOF){
                ret = 0;
                break;
            }
            char error_msg[1000];
            av_strerror(ret, error_msg, 1000);
            puts(error_msg);
        }
        if(pkt.stream_index == video_stream){

            ret = decode(codecCtx, frame, &pkt);
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
                if(yuvFrame == NULL){
                    fprintf(stdout, "VisYUVFrame_Create failed\n");
                    exit(1);
                }
                VisYUVFrame_SetSize(yuvFrame, frame->width, frame->height);
                if((ret = ffmpeg2visframe(yuvFrame, frame)) !=0){
                    fprintf(stdout, "ffmpeg2visframe failed\n");
                    exit(ret);
                }

                visProcessContext proCtx;
                proCtx.processCb = visVisResult_CaculateBrightestOverWidth;
                visVisualResult     result;
                if((ret = VisVisualResult_Init(&result)) != 0 ){
                    fprintf(stderr, "Unable to initialize a visVisualResult with code %d\n", ret);
                    exit(1);
                }


                if((ret = VisVisualResult_SetSize(&result, frame_width)) != 0){
                    fprintf(stderr, "VisVisualResult_SetSize failed with code %d\n", ret);
                    exit(1);
                }



                if((ret = visVisProcess(&result, yuvFrame, &proCtx, sliceBuffer)) != 0){
                    fprintf(stdout, "visVisProcess failed\n");
                    exit(ret);
                }


                if(codecCtx->frame_number % 100 == 0){
                    fprintf(stdout, "Adding frame %d to buffer\n", codecCtx->frame_number);
                }
                if((ret = visBuffer_PushBackResult(buffer, &result)) != 0){
                    fprintf(stdout, "visBuffer_PushBackResult failed\n");
                    exit(ret);
                }

                VisVisualResult_Cleanup(&result);
                VisYUVFrame_Destroy(&yuvFrame);
            }

        }
        av_packet_unref(&pkt);
        if(codecCtx->frame_number >= MAX_BUFFER_SIZE){
            break;
        }

    }
    free(sliceBuffer);
    av_frame_free(&frame);
    avformat_close_input(&avFormatCtx);
    visView *view = VisView_Create(buffer->bufferWidth, codecCtx->frame_number);
    av_parser_close(parser);
    avcodec_free_context(&codecCtx);
    if(view == NULL){
        fprintf(stderr, "Unable to allocate a visView\n");
        exit(1);
    }

    if((ret = visView_Update4(view, buffer)) != 0){
        fprintf(stdout, "visView_Update4 failed\n");
        exit(ret);
    }
    puts("Rasterizing visualization");

    fprintf(stdout, "Writing visualization to file %s\n", outfilename);
    visImage image;
    visImage_Alloc(&image, view->width, view->height, 1);
    if((ret = visView_GenerateBW(&image, view)) != 0){
        fprintf(stderr, "visView_GenerateBW failed with value %d\n",  ret);
        exit(1);

    }

    VisView_Destroy(&view);
    VisBuffer_Destroy(&buffer);


    avformat_close_input(&avFormatCtx);

    image_save_pgm(&image, outfilename);
    visImage_FreeData(&image);



    return ret >= 0 ? 0: 1;
}

