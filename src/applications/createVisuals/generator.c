//
// Created by Borchers, Henry Samuel on 12/3/16.
//

#include "generator.h"
#include <SDL2/SDL_events.h>
#include <stdio.h>
#include "gui.h"
#include "ramp.h"
#include "visImageWriter.h"

#define SCREENWIDTH 640
#define SCREENHEIGHT 480


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