//
// Created by Borchers, Henry Samuel on 12/3/16.
//

#ifndef LIBVISVID_VIDEOANDVISUALPLAYER_H
#define LIBVISVID_VIDEOANDVISUALPLAYER_H

//
//#include "SDL2/SDL.h"
//#include "visImageWriter.h"
//#include "libavutil/frame.h"
#include <SDL_render.h>
#include <SDL_video.h>
#include <visvid/visvid.h>
#include "videoDecoder.h"

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
    visImageRGB     buffer;
}DisplayWidgetContext;
void vidVis_ctx_init(DisplayWidgetContext *ctx);
int vidVis_ctx_init2(DisplayWidgetContext *ctx, const DecoderContext* decoderCtx);

int playVideoVisInit();
int vidVis_build_window(DisplayWidgetContext *ctx, int videoWidth, int videoHeight);
int vidVis_open_window(DisplayWidgetContext *ctx);
void vidVis_cleanup();
int playVideoVis(DecoderContext *decoder, DisplayWidgetContext *vidCtx);
void vidVis_destroy_window(DisplayWidgetContext *ctx);


#endif //LIBVISVID_VIDEOANDVISUALPLAYER_H
