//
// Created by Borchers, Henry Samuel on 11/11/16.
//

#ifndef VISVID_GUI_H
#define VISVID_GUI_H

#include <SDL_video.h>
#include <SDL_render.h>
#include "visImageWriter.h"

typedef struct {
    int             windowWidth;
    int             windowHeight;
    char            *sourceFilename;
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    SDL_Texture     *texture;
}gui_context;

int gui_init();
void gui_ctx_init(gui_context *ctx);
int gui_build_window(gui_context *ctx);
void gui_destroy_window(gui_context *ctx);

int gui_refresh(gui_context *ctx, visImageRGB *texture);
#endif //VISVID_GUI_H
