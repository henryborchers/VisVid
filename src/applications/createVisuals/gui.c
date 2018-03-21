//
// Created by Borchers, Henry Samuel on 11/11/16.
//

#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <stdio.h>
#include <string.h>
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
    ctx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, ctx->windowWidth, ctx->windowHeight);
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

int gui_refresh(gui_context *ctx, visImageRGB *texture) {

    int res = 0;

    int pitch;
    void *pixels;
    res = SDL_LockTexture(ctx->texture, NULL, &pixels, &pitch);

    memcpy(pixels, texture->plane, (pitch * texture->height));

    SDL_UnlockTexture(ctx->texture);


    if(res != 0){
        fprintf(stderr, "%s\n", SDL_GetError());
    }
    SDL_RenderCopy(ctx->renderer, ctx->texture, NULL, NULL);

    SDL_RenderPresent(ctx->renderer);
    return 0;
}
