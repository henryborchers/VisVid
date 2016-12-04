//
// Created by Borchers, Henry Samuel on 12/3/16.
//

#include <stdbool.h>
#include "videoDecoder.h"
#include "runPlayers.h"
#include "ramp.h"

static int vidVis_init();
static void vidVis_ctx_init(VidVisContext *ctx);
static int vidVis_build_window(VidVisContext *ctx, int videoWidth, int videoHeight);
static void vidVis_destroy_window(VidVisContext *ctx);
static int vidVis_refresh(VidVisContext *ctx, AVFrame *pFrame, visImageRGB *texture);
static void vidVis_cleanup();

static bool quitCalled(SDL_Event *pEvent);

int playVideoVis(const char *filename) {
    DecoderContext      *decoderCtx = NULL;
    AVFrame             *frame = NULL;
    VidVisContext       ctx;
    SDL_Event           event;
    int                 res;
    visImageRGB         visualization;

    decoder_init();

    if((res = vidVis_init()) !=0){
        fprintf(stderr, "ERROR: Unable to initialized SDL\n");
        return res;
    };


    puts("Opening video");
    decoderCtx = decoderContext_Create(filename);
    if(decoderCtx == NULL){
        fprintf(stderr, "Unable to load file \"%s\"", filename);
        return 1;
    } else{
        puts("Video Opened");
    }

    int videoHeight;
    int videoWidth;
    decoderContext_GetSize(decoderCtx, &videoWidth, &videoHeight);

    puts("Initializing context");
    vidVis_ctx_init(&ctx);


    puts("Initializing visualization image");
    if((res = visImageRGB_Alloc(&visualization, videoWidth, videoWidth)) != 0){
        fprintf(stderr, "visImageRGB_Alloc failed\n");
        return res;
    };




    puts("Building Window");

    if((res = vidVis_build_window(&ctx, videoWidth, videoHeight)) != 0){
        fprintf(stderr, "Building Window failed with error code %d\n ", res);
        return res;
    };

    while(1){
        if(quitCalled(&event)){
            break;
        }

        res = decoderContext_NextFrame(decoderCtx, &frame);
        ramp(&visualization);
        if(NULL == frame){
            decoderContext_Rewind(decoderCtx);
            continue;
        }
        if(res < 0 || frame == NULL){
            break;
        }
        vidVis_refresh(&ctx, frame, &visualization);
    }

    puts("Destroying Window");
    vidVis_destroy_window(&ctx);

    vidVis_cleanup();
    return 0;
}

bool quitCalled(SDL_Event *pEvent) {
    while(SDL_PollEvent(pEvent) != 0){
        if(pEvent->type == SDL_QUIT){
            return true;
        };

    }
    return false;
}

void vidVis_cleanup() {
    SDL_Quit();
}

int vidVis_init() {
    int res = 0;

    res = SDL_Init(SDL_INIT_EVERYTHING);
    if(res != 0){

        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());

        return res;
    }
    return 0;
}

void vidVis_ctx_init(VidVisContext *ctx) {

    if(NULL == ctx){
        return;
    }

    ctx->renderer               = NULL;
    ctx->window                 = NULL;

    ctx->windowHeight           = -1;
    ctx->windowWidth            = -1;

    ctx->visualization.texture  = NULL;
    ctx->visualization.height   = -1;
    ctx->visualization.width    = -1;
    ctx->visualization.x        = -1;
    ctx->visualization.y        = -1;

    ctx->video.texture          = NULL;
    ctx->video.height           = -1;
    ctx->video.width            = -1;
    ctx->video.x                = -1;
    ctx->video.y                = -1;

    return;
}

int vidVis_build_window(VidVisContext *ctx, int videoWidth, int videoHeight) {
    int res;

    int window_width = videoWidth;
    int window_height = videoWidth + videoHeight;

    // Build the window
    printf("Creating window and renderer, %d x %d\n", window_width, window_height);

    if(NULL != ctx->window){
        fprintf(stderr, "ctx->window is not null\n");
    }

    if(NULL != ctx->renderer){
        fprintf(stderr, "ctx->renderer is not null\n");
    }

    ctx->window = SDL_CreateWindow("Dummy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_RESIZABLE);
    if(ctx->window == NULL){
        fprintf(stderr, "Unable to create window");
        return -1;
    };
    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, 0);
    if(ctx->renderer == NULL ){
        fprintf(stderr, "Unable to create renderer");
        return -1;
    }
//    if((res = SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOW_RESIZABLE, &ctx->window, &ctx->renderer)) != 0){
//        return res;
//    };
    ctx->windowWidth = window_width;
    ctx->windowHeight = window_height;


    // Build the visualization widget
    puts("Creating visualization texture");
    ctx->visualization.texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, videoWidth, videoWidth);
    if(NULL == ctx->visualization.texture){
        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->window);
        return -1;
    }
    ctx->visualization.width = videoWidth;
    ctx->visualization.height = videoWidth;
    ctx->visualization.x = 0;
    ctx->visualization.y = videoHeight;

    // Build the video player
    puts("Creating video texture");
    ctx->video.texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, videoWidth, videoHeight);
    if(NULL == ctx->video.texture){
        SDL_DestroyTexture(ctx->visualization.texture);
        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->window);
        return -1;
    }
    ctx->video.width = videoWidth;
    ctx->video.height = videoHeight;
    ctx->video.x = 0;
    ctx->video.y = 0;

    puts("Build successful");
    return 0;
}

void vidVis_destroy_window(VidVisContext *ctx) {
    puts("Destroying widgets");
    if(NULL != ctx->video.texture){
        SDL_DestroyTexture(ctx->video.texture);
    }
    ctx->video.height = -1;
    ctx->video.width = -1;
    ctx->video.x = -1;
    ctx->video.y = -1;

    if(NULL != ctx->visualization.texture) {
        SDL_DestroyTexture(ctx->visualization.texture);
    }
    ctx->visualization.height = -1;
    ctx->visualization.width = -1;
    ctx->visualization.x = -1;
    ctx->visualization.y = -1;

    puts("Destroying renderer");
    SDL_DestroyRenderer(ctx->renderer);
    puts("Destroying window");
    SDL_DestroyWindow(ctx->window);
    puts("Destroying complete");


}

int vidVis_refresh(VidVisContext *ctx, AVFrame *pFrame, visImageRGB *texture) {
    int res = 0;
    SDL_Rect videoOrig;
    SDL_Rect videoWidget;
    SDL_Rect visualizationOrig;
    SDL_Rect visualizationWidget;
    int rendX = 0;
    int rendY = 0;

    float scaleX = 0;
    float scaleY = 0;

    SDL_GetRendererOutputSize(ctx->renderer, &rendX, &rendY);

    videoOrig.x = ctx->video.x;
    videoOrig.y = ctx->video.y;
    videoOrig.w = ctx->video.width;
    videoOrig.h = ctx->video.height;

    visualizationOrig.x = ctx->visualization.x;
    visualizationOrig.y = ctx->visualization.y;
    visualizationOrig.w = ctx->visualization.width;
    visualizationOrig.h = ctx->visualization.height;


    // FIXME: Scaling will break if window width or height changes
    scaleX = (float)rendX/ctx->windowWidth;
    scaleY = (float)rendY/(ctx->windowHeight);

    videoWidget.x = 0;
    videoWidget.y = 0;
    videoWidget.h = (int)(videoOrig.h * scaleY);
    videoWidget.w = (int)(videoOrig.w * scaleX);

    visualizationWidget.x = (int)(visualizationOrig.x * scaleX);
    visualizationWidget.y = (int)(visualizationOrig.y * scaleY);
    visualizationWidget.h = (int)(visualizationOrig.h * scaleY);
    visualizationWidget.w = (int)(visualizationOrig.w * scaleX);

    SDL_RenderClear(ctx->renderer);
    if((res = SDL_UpdateYUVTexture(ctx->video.texture, &videoOrig,
                                   pFrame->data[0], pFrame->linesize[0],
                                   pFrame->data[1], pFrame->linesize[1],
                                   pFrame->data[2], pFrame->linesize[2])) != 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return res;
    };

    SDL_UpdateTexture(ctx->visualization.texture, NULL, texture->plane, texture->pitch);

    SDL_RenderCopy(ctx->renderer, ctx->visualization.texture, &visualizationOrig, &visualizationWidget);
    SDL_RenderCopy(ctx->renderer, ctx->video.texture, &videoOrig, &videoWidget);
    SDL_RenderPresent(ctx->renderer);
    return 0;
}
