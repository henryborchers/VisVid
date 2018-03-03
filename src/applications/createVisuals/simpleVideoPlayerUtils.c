//
// Created by Borchers, Henry Samuel on 12/3/16.
//
#include <SDL2/SDL.h>
#include <avformat.h>
#include "simpleVideoPlayerUtils.h"

int player_init() {
    int res = 0;
    res = SDL_Init(SDL_INIT_EVERYTHING);
    if(res != 0){
        fprintf(stderr, "SDL Error\n");
        return -1;
    }
    return 0;
}

void player_ctx_init(PlayerContext *ctx) {
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

int player_build_window(PlayerContext *ctx) {
    int res = 0;
    res = SDL_CreateWindowAndRenderer(ctx->windowWidth, ctx->windowHeight, SDL_WINDOW_RESIZABLE, &ctx->window, &ctx->renderer);
    ctx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, ctx->windowWidth, ctx->windowHeight);
    if(res != 0){
        fprintf(stderr, "Unable to create SDL Window\n");
        return -1;
    }
    SDL_RenderClear(ctx->renderer);
    return 0;
}

int player_refresh(PlayerContext *ctx, AVFrame *pFrame) {
    int res = 0;
    if((res = SDL_UpdateYUVTexture(ctx->texture, NULL,
                                   pFrame->data[0], pFrame->linesize[0],
                                   pFrame->data[1], pFrame->linesize[1],
                                   pFrame->data[2], pFrame->linesize[2])) != 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return res;
    };

    SDL_RenderCopy(ctx->renderer, ctx->texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);
    return 0;
}

void player_destroy_window(PlayerContext *ctx) {
    puts("Destroying texture");
    SDL_DestroyTexture(ctx->texture);
    puts("Destroying renderer");
    SDL_DestroyRenderer(ctx->renderer);
    puts("Destroying window");
    SDL_DestroyWindow(ctx->window);

}


