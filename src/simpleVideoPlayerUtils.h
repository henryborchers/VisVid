//
// Created by Borchers, Henry Samuel on 12/3/16.
//

#ifndef LIBVISVID_PLAYER_H
#define LIBVISVID_PLAYER_H


typedef struct {
    int             windowWidth;
    int             windowHeight;
    char            *sourceFilename;
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    SDL_Texture     *texture;
}PlayerContext;

int player_init();
void player_ctx_init(PlayerContext *ctx);
int  player_build_window(PlayerContext *ctx);
void player_destroy_window(PlayerContext *ctx);

int player_refresh(PlayerContext *ctx, AVFrame *pFrame);

#endif //LIBVISVID_PLAYER_H