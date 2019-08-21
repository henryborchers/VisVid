//
// Created by Borchers, Henry Samuel on 12/3/16.
//

//#include <libavutil/avutil.h>
#include "simpleVideoPlayer.h"
#include <SDL_events.h>
//#include <SDL2/SDL.h>
#include "simpleVideoPlayerUtils.h"
#include "videoDecoder.h"
#include <libavutil/frame.h>
#include <stdio.h>

int playVideoSimple(const char *filename) {
    AVFrame             *frame = NULL;
    SDL_Event           event;
    PlayerContext       playerContext;
    int                 ret;

    decoder_init();

    // initialize player
    player_init();

    puts("Opening video");
    DecoderContext *decoderCtx =  decoderContext_Create(filename);

    if(decoderCtx == NULL){
        fprintf(stderr, "Unable to load file \"%s\"", filename);
        return 1;
    } else{
        puts("Video Opened");
    }

    // Load gui
    player_ctx_init(&playerContext);
    decoderContext_GetSize(decoderCtx, &playerContext.windowWidth, &playerContext.windowHeight);
    player_build_window(&playerContext);

    while(1){
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            break;
        }

        ret = decoderContext_NextFrame(decoderCtx, &frame);
        if(ret < 0 || frame == NULL){
            break;
        }
        player_refresh(&playerContext, frame);
    }
    puts("Closing window");
    player_destroy_window(&playerContext);

    puts("Closing video ");
    decoderContext_Destroy(&decoderCtx);

    return 0;
}