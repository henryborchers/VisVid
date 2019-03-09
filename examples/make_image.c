//
// Created by henry on 3/3/19.
//
#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include <visvid/visvid.h>


const char outfilename[] = "dummy.pgm";

// TODO: create a callback for adding to the buffer
static void decode(AVCodecContext *dec_cts, AVFrame *frame, AVPacket *ptk){
    int ret;

    ret = avcodec_send_packet(dec_cts, ptk);
    if(ret < 0 ){
        char error_msg[1000];
        av_strerror(ret,error_msg, 1000);
        fprintf(stderr, "error sending a packet for decoding. Reason: %s\n", error_msg);
        exit(1);
    }
    while (ret >=0){
        ret = avcodec_receive_frame(dec_cts, frame);
        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            return;
        } else if (ret < 0){
            fprintf(stderr, "error During decoding\n");
            exit(1);
        }
        fprintf(stderr, "Frame %d\n", dec_cts->frame_number);
    }

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
    av_register_all();
    if(( ret = avformat_open_input(&avFormatCtx, filename, NULL, NULL))){
        fprintf(stderr, "unable to open %s\n", filename);
        return ret;
    }
    for (int stream_number = 0; stream_number < avFormatCtx->nb_streams; stream_number++) {
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

    FILE *f = fopen(filename, "rb");
    if(!f){
        fprintf(stderr, "Unable to open file\n");
        exit(1);
    }
    printf("Using %s\n",filename);
    visBuffer *buffer = VisBuffer_Create2(avFormatCtx->streams[video_stream]->codecpar->width, 100);
    if(buffer == NULL){
        fprintf(stderr, "Unable to allocate a visbuffer\n");
        exit(1);
    }
    while(1) {
        if((ret = av_read_frame(avFormatCtx, &pkt)) < 0){
            if(ret != AVERROR_EOF){
                char error_msg[1000];
                av_strerror(ret, error_msg, 1000);
                puts(error_msg);
            }
            break;
        }
        if(pkt.stream_index == video_stream){
            decode(codecCtx, frame, &pkt);

        }
        av_packet_unref(&pkt);

    }
    VisBuffer_Destroy(&buffer);
    fclose(f);
    av_parser_close(parser);
    avcodec_free_context(&codecCtx);
    av_frame_free(&frame);
    avformat_close_input(&avFormatCtx);

    return 0;
}