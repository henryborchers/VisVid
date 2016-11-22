//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include "gui.h"
#include "textures.h"
#include "visVersion.h"

#define SCREENWIDTH 640
#define SCREENHEIGHT 480


int main(int argc, char *argv[]){

    puts("Using VisVis library");
    printf("Version %s\n", VERSION_VIS_S);

    gui_context gui;
    SDL_Event event;
//    SDL_Rect r;
    VisTexture t;

//    r.h = 100;
//    r.w = 100;
//    r.x = 10;
//    r.y = 10;


    puts("Hello");
    puts("Creating texture");

    createRedBox(&t, SCREENWIDTH, SCREENHEIGHT);

    if(gui_init() != 0){
        return 1;
    }

    gui_ctx_init(&gui);

    gui.windowWidth = SCREENWIDTH;
    gui.windowHeight = SCREENHEIGHT;
//    SDL_CreateTexture(gui.renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STATIC,gui.windowWidth, gui.windowHeight );
    gui_build_window(&gui);


    while(1){
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            break;
        }
        modify(&t);
        gui_refresh(&gui, &t);
//        SDL_Delay(10);
    }

    gui_destroy_window(&gui);

}
