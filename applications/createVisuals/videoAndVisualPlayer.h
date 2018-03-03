//
// Created by Borchers, Henry Samuel on 12/3/16.
//

#ifndef LIBVISVID_VIDEOANDVISUALPLAYER_H
#define LIBVISVID_VIDEOANDVISUALPLAYER_H


#include "SDL2/SDL.h"
#include "visImageWriter.h"
#include "libavutil/frame.h"

typedef struct {
    SDL_Texture *texture;
    int         width;
    int         height;
    int         x;
    int         y;
} VidVisWidget;

typedef struct {
    int             windowWidth;
    int             windowHeight;
    SDL_Window      *window;
    SDL_Renderer    *renderer;
    VidVisWidget    visualization;
    VidVisWidget    video;
}VidVisContext;




int playVideoVis(const char *filename);



#endif //LIBVISVID_VIDEOANDVISUALPLAYER_H
