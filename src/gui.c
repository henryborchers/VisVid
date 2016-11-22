//
// Created by Borchers, Henry Samuel on 11/11/16.
//

#include <stdio.h>
#include "gui.h"
#include "textures.h"
#include <SDL2/SDL.h>
void say_hi() {
    puts("hi");
}

int gui_init() {
    int res = 0;
    res = SDL_Init(SDL_INIT_EVERYTHING);
    if(res != 0){
        fprintf(stderr, "SDL Error\n");
        return -1;
    }
    return 0;
}

void gui_ctx_init(gui_context *ctx) {
    if(NULL == ctx){
        return;
    }

    ctx->renderer           = NULL;
    ctx->sourceFilename     = NULL;
    ctx->window             = NULL;
    ctx->texture            = NULL;
    ctx->windowHeight       = -1;
    ctx->windowWidth        = -1;
}

int gui_build_window(gui_context *ctx) {
    int res = 0;
    res = SDL_CreateWindowAndRenderer(ctx->windowWidth, ctx->windowHeight, SDL_WINDOW_RESIZABLE, &ctx->window, &ctx->renderer);
    ctx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STATIC, ctx->windowWidth, ctx->windowHeight);
    if(res != 0){
        fprintf(stderr, "Unable to create SDL Window\n");
        return -1;
    }
    SDL_RenderClear(ctx->renderer);
    return 0;
}



void gui_destroy_window(gui_context *ctx) {
    SDL_DestroyTexture(ctx->texture);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
}

int gui_refresh(gui_context *ctx, VisTexture *texture) {
    SDL_Rect r;
    r.y = 0;
    r.x = 0;
    r.w = texture->width;
    r.h = texture->height;

//    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
//    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx->renderer);

    SDL_UpdateYUVTexture(ctx->texture, NULL,
                         texture->yPlane, texture->yPitch,
                         texture->uPlane, texture->uPitch,
                         texture->vPlane, texture->vPitch);
    SDL_RenderCopy(ctx->renderer, ctx->texture, &r, NULL);
//    SDL_RenderCopy(ctx->renderer, ctx->texture, &r, &r);
    SDL_RenderPresent(ctx->renderer);
    return 0;
}
