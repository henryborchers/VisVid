//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <libavutil/frame.h>
#include "gui.h"
#include "generators.h"
#include "visVersion.h"
#include "videoDecoder.h"

#define SCREENWIDTH 640
#define SCREENHEIGHT 480


int generate_waves();

int main(int argc, char *argv[]){

    DecoderContext      *decoderCtx = NULL;
    AVFrame             *frame = NULL;
    decoder_init();

    puts("Using VisVis library");
    printf("Version %s\n", VERSION_VIS_S);
    fflush(stdout);

    // Check args
    if(argc != 2){
        fprintf(stderr, "You need a file name to continue\n");
        return 1;
    }


    puts("Opening video");
    decoderCtx = decoderContext_Create(argv[1]);
    if(decoderCtx == NULL){
        fprintf(stderr, "Unable to load file \"%s\"", argv[1]);
        return 1;
    } else{
        puts("Video Opened");
    }

    // loop over video until hitting an error or the end of the file
    while(decoderContext_NextFrame(decoderCtx, &frame) >= 0){

        // TODO: Render out frame

    };

    puts("Closing video ");
    decoderContext_Destroy(&decoderCtx);

}

int generate_waves() {
    gui_context gui;
    SDL_Event event;
//    SDL_Rect r;
    visImageRGB t;

//    r.h = 100;
//    r.w = 100;
//    r.x = 10;
//    r.y = 10;


    puts("Hello");
    puts("Creating RGBA Image");

    visImageRGB_Alloc(&t, SCREENWIDTH, SCREENHEIGHT);

    if(gui_init() != 0){
        return 1;
    }

    gui_ctx_init(&gui);

    gui.windowWidth = SCREENWIDTH;
    gui.windowHeight = SCREENHEIGHT;
    gui_build_window(&gui);


    while(1){
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            break;
        }
        ramp(&t);
        gui_refresh(&gui, &t);
    }
    puts("Deleting RGBA Image");
    gui_destroy_window(&gui);
    return 0;
}
