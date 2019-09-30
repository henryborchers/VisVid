//
// Created by Borchers, Henry Samuel on 12/3/16.
//

//#include <stdbool.h>
//#include <visBuffer.h>
//#include <visFrame.h>
//#include <visVisualization.h>
//#include <visView.h>
//#include "videoDecoder.h"
//#include "runPlayers.h"
//#include "ramp.h"
//#include "ffmpeg_converter.h"
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_log.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <libavutil/frame.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <visvid/visvid.h>
//#include <visBuffer.h>
//#include <visFrame.h>
//#include <visView.h>
//#include <visVisualization.h>
#include "ffmpeg_converter.h"
#include "videoAndVisualPlayer.h"
#include "videoDecoder.h"
//#include "visImageWriter.h"
//#include "visResult.h"
//#include "visTypes.h"
#define NUM_COLORS 4

static int vidVis_init();

static int setup_visualization_widget(VidVisWidget *visCtx, const DisplayWidgetContext *ctx, const DecoderContext *decoderCtx);
static int setup_video_widget(VidVisWidget *videoCtx, const DisplayWidgetContext *ctx, const DecoderContext *decoderCtx);
static int setup_window(DisplayWidgetContext *windowCtx, const DecoderContext *decoderCtx, const char* title);
static int vidVis_refresh(DisplayWidgetContext *ctx, VidVisWidget *visWidgetCtx,VidVisWidget *videoWidgetCtx, AVFrame *pFrame, visImageRGB *texture);
static int update_video_widget(const DisplayWidgetContext *ctx,VidVisWidget *widget, AVFrame *pFrame);

//static int variable_color(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);
//static bool quitCalled(SDL_Event *pEvent);
//static float min(float d, float d1);
//static float enforce_range(float min, float max, float value);
//static int generate_fakeresult(visVisualResult *pResult);

static uint8_t offset = 255;
static float offset_color_midtones = 1;
static float offset_color_highlights = 1;

int playVideoVisInit(){
    decoder_init();

    int rc = 0;
    if((rc = vidVis_init()) !=0){
        fprintf(stderr, "ERROR: Unable to initialized SDL\n");
        return rc;
    };


    return rc;
}

int playVideoVis(DecoderContext *decoder, DisplayWidgetContext *vidCtx, VidVisWidget *visWidget, VidVisWidget *videoWidget) {
    DecoderContext      *decoderCtx = decoder;
    AVFrame             *frame = NULL;
    visProcessContext   processContext;
    visView             *view = NULL;
    VisYUVFrame         *frameBuffer = NULL;
    visBuffer           *buffer = NULL;
    SDL_Event           event;
    int                 rc;
    visVisualResult     result;

    int                 returncode = 0;

    puts("Opening video");

    int videoHeight;
    int videoWidth;
    decoderContext_GetSize(decoderCtx, &videoWidth, &videoHeight);

    puts("Initializing visualization image");
    if((rc = visImageRGB_Alloc(&vidCtx->buffer, videoWidth, videoWidth)) != 0){
        fprintf(stderr, "visImageRGB_Alloc failed\n");
        return rc;
    };


    // build buffer with the size of the frame
    puts("Creating buffer");
    if((buffer = VisBuffer_Create2((size_t) videoWidth, (size_t)videoWidth)) == NULL){
        fprintf(stderr, "Failed to create a New buffer");
        return 1;
    };

    puts("Creating analysis frame buffer");
    if((frameBuffer = VisYUVFrame_Create()) == NULL){
        fprintf(stderr, "Failed to create a frame Buffer");
        return 1;
    };
    puts("Setting frame buffer");
    VisYUVFrame_SetSize(frameBuffer, videoWidth, videoHeight);


    puts("Creating view");

    view = VisView_Create(videoWidth, videoWidth);

    puts("Configuring the process context");

    // TODO: fix this API so that this doesn't have to be assigned this way.
    processContext.processCb = visVisResult_CaculateBrightestOverWidth;

    puts("Starting decoding loop");
    if((rc = VisVisualResult_Init(&result)) != 0 ) {
        fprintf(stderr, "VisVisualResult_Init Failed with code %d\n", rc);
        return rc;
    }

    //             Configure the size of the result data set
    if((rc = VisVisualResult_SetSize(&result, (size_t )videoWidth)) != 0){
        fprintf(stderr, "VisVisualResult_SetSize Failed\n");
        return rc;
    }


    PixelValue *slice = malloc(sizeof(int) * videoWidth);
    while(1){
        while(SDL_PollEvent(&event) != 0){
            switch(event.type){
                case SDL_QUIT:
                    goto stopmainloop;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_DOWN:
                            if(offset > 10){
                                offset -= 10;
                            }
                            if(offset_color_midtones > .1){
                                offset_color_midtones -= .1;
                            }
                            break;

                        case SDLK_UP:
                            if(offset_color_midtones < .9){
                                offset_color_midtones += .1;
                            }
                            break;
                        case SDLK_RIGHT:
                            if(offset < 245){
                                offset+= 10;
                            }
                            if(offset_color_highlights < .9){
                                offset_color_highlights += .1;
                            }
                            break;
                        case SDLK_LEFT:
                            if(offset_color_highlights > .1){
                                offset_color_highlights -= .1;
                            }
                            break;
                        case SDLK_q:
                            puts("quit");
                            goto stopmainloop;
                        default:break;
                    }
                default:break;
            }

        }


        // Get a frame
        rc = decoderContext_NextFrame(decoderCtx, &frame);

        // If no frame is retrieved, rewind the video
        if(NULL == frame){
            fprintf(stderr, "rewinding\n");
            decoderContext_Rewind(decoderCtx);
            continue;
        }

        // If there is an error with decoding, stop!
        if(rc < 0 || frame == NULL){
            returncode = rc;
            break;
        }

        // Convert the ffmpeg frame into a frame that can be analyzed
        if((rc = ffmpeg2visframe(frameBuffer, frame)) != 0){
            fprintf(stderr, "ffmpeg2visframe failed with error code %d.\n", rc);
            returncode = rc;
            break;
        } else {

            // process the frame and save the data to a result data set
            if((rc = visVisProcess(&result, frameBuffer, &processContext, slice)) != 0){
                returncode = rc;
                fprintf(stderr, "visVisProcess Failed\n");
                break;
            };

            // Add result data set to buffer
            if((rc =  visBuffer_PushBackResult(buffer, &result)) != 0){
                returncode = rc;
                fprintf(stderr, "visBuffer_PushBackResult Failed\n");
                break;
            };

            // update a view of the buffer
            if((rc = visView_Update4(view, buffer)) != 0){
                returncode = rc;
                fprintf(stderr, "visView_Update Failed with code %d.\n", rc);
                break;
            }
//            // Render a picture of the view to the visualization image
            if((rc = visViewRGB_GenerateRGBA(&vidCtx->buffer, view, visViewRGBA_value2color1)) != 0){
                returncode = rc;
                fprintf(stderr, "visViewRGB_GenerateRGBA Failed with code %d.\n", rc);
                break;
            }
        }


        vidVis_refresh(vidCtx, visWidget, videoWidget, frame, &vidCtx->buffer);
    }
    stopmainloop:
    free(slice);
    VisVisualResult_Cleanup(&result);

    visImageRGB_FreeData(&vidCtx->buffer);

    puts("Destroying Frame buffer");
    VisYUVFrame_Destroy(&frameBuffer);

    puts("Destroying buffer");
    VisBuffer_Destroy(&buffer);



    puts("Destroying view");
    VisView_Destroy(&view);

    vidVis_cleanup();
    return returncode;
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

void vidVis_destroy_window(DisplayWidgetContext *ctx) {

    puts("Destroying renderer");
    SDL_DestroyRenderer(ctx->renderer);
    puts("Destroying window");
    SDL_DestroyWindow(ctx->window);
    puts("Destroying complete");


}

int setup_window(DisplayWidgetContext *windowCtx, const DecoderContext *decoderCtx, const char* title){
    int videoWidth;
    int videoHeight;
    decoderContext_GetSize(decoderCtx, &videoWidth, &videoHeight);
    windowCtx->windowWidth = videoWidth/2;
    windowCtx->windowHeight = (videoWidth + videoHeight)/2;
    windowCtx->window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            windowCtx->windowWidth,
            windowCtx->windowHeight,
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN );
    if(windowCtx->window == NULL){
        fprintf(stderr, "Unable to create window");
        return -1;
    };
    if(NULL != windowCtx->renderer){
        fprintf(stderr, "ctx->renderer is not null\n");
    }
    windowCtx->renderer = SDL_CreateRenderer(windowCtx->window, -1, 0);
    if(windowCtx->renderer == NULL ){
        fprintf(stderr, "Unable to create renderer");
        return -1;
    }
    return 0;

}

int setup_visualization_widget(VidVisWidget *visCtx, const DisplayWidgetContext *ctx, const DecoderContext *decoderCtx) {
    puts("Creating visualization texture");
    int img_width;
    int img_height;

    decoderContext_GetSize(decoderCtx, &img_width, &img_height );
    visCtx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, img_width, img_width);
    if(NULL == visCtx->texture){
        return -1;
    }
    visCtx->width = img_width;
    visCtx->height = img_width;
    visCtx->x = 0;
    visCtx->y = 0;


    return 0;
}
int setup_video_widget(VidVisWidget *videoCtx, const DisplayWidgetContext *ctx, const DecoderContext *decoderCtx){
    int videoHeight;
    int videoWidth;
    decoderContext_GetSize(decoderCtx, &videoWidth, &videoHeight);

    puts("Creating video texture");
    videoCtx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, videoWidth, videoHeight);

    if(NULL == videoCtx->texture){
        return -1;
    }
    videoCtx->width = videoWidth;
    videoCtx->height = videoHeight;
    videoCtx->x = 0;
    videoCtx->y = 0;
    return 0;
}
int vidVis_ctx_init(DisplayWidgetContext *windowCtx, VidVisWidget *visWidget, VidVisWidget *videoWidget,
                    const DecoderContext *decoderCtx) {

    int return_code = 0;

    windowCtx->renderer               = NULL;
    windowCtx->window                 = NULL;

    windowCtx->windowHeight           = -1;
    windowCtx->windowWidth            = -1;

    if((return_code = setup_window(windowCtx, decoderCtx, "DUMMY") != 0)){
        return return_code;
    }

    if((return_code = setup_visualization_widget(visWidget, windowCtx, decoderCtx) != 0)){
        SDL_DestroyRenderer(windowCtx->renderer);
        SDL_DestroyWindow(windowCtx->window);
        return return_code;
    }
    if((return_code = setup_video_widget(videoWidget, windowCtx, decoderCtx) != 0)){
        SDL_DestroyTexture(visWidget->texture);
        SDL_DestroyRenderer(windowCtx->renderer);
        SDL_DestroyWindow(windowCtx->window);
        return return_code;
    }
    return 0;
}

int vidVis_open_window(DisplayWidgetContext *ctx) {
    SDL_ShowWindow(ctx->window);
    return 0;
}


int update_visualization_widget(const DisplayWidgetContext *ctx, VidVisWidget *widget, visImageRGB *texture, float scaleY,
                                float scaleX) {
    int windowHeight;
    int windowWidth;
    SDL_GetWindowSize(ctx->window, &windowWidth, &windowHeight);

    SDL_Rect visualizationOrig;
    SDL_Rect visualizationWidget;

    visualizationOrig.x = widget->x;
    visualizationOrig.y = widget->y;
    visualizationOrig.w = widget->width;
    visualizationOrig.h = widget->height;

    // Calculate how large the visualization should be when scaled based on the window
    visualizationWidget.x = (int)(visualizationOrig.x * scaleX);
    visualizationWidget.y = windowHeight - (widget->height* scaleY);
    visualizationWidget.h = windowHeight-(int)round(((float)ctx->windowWidth * scaleY));
    visualizationWidget.w = (int)(widget->width * scaleX);

    SDL_UpdateTexture(widget->texture, &visualizationOrig , texture->plane, texture->pitch);
    // Draw the visualization
    SDL_RenderCopy(ctx->renderer, widget->texture, &visualizationOrig, &visualizationWidget);
    return 0;
}
int update_video_widget(const DisplayWidgetContext *ctx, VidVisWidget *widget, AVFrame *pFrame) {
    int windowHeight;
    int windowWidth;
    SDL_GetWindowSize(ctx->window, &windowWidth, &windowHeight);
    float scaleX = (float)windowWidth / (float)pFrame->width;
    float scaleY = (float)windowHeight / (float)(pFrame->width + pFrame->height);


    SDL_Rect videoOrig;
    SDL_Rect videoWidget;

    videoOrig.x = widget->x;
    videoOrig.y = widget->y;
    videoOrig.w = pFrame->width;
    videoOrig.h = pFrame->height;
    // Calculate how large the video player should be when scaled based on the window

    videoWidget.x = 0;
    videoWidget.y = 0;
    videoWidget.h = (int)round(((float)videoOrig.h) * scaleY);
    videoWidget.w = (int)(((float)videoOrig.w * scaleX));
    int res = 0;
    if((res = SDL_UpdateYUVTexture(widget->texture, &videoOrig,
                                   pFrame->data[0], pFrame->linesize[0],
                                   pFrame->data[1], pFrame->linesize[1],
                                   pFrame->data[2], pFrame->linesize[2])) != 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return res;
    }
    // Draw the video
    SDL_RenderCopy(ctx->renderer, widget->texture, &videoOrig, &videoWidget);
    return res;
}

int vidVis_refresh(DisplayWidgetContext *ctx, VidVisWidget *visWidgetCtx, VidVisWidget *videoWidgetCtx, AVFrame *pFrame, visImageRGB *texture) {
    int res = 0;

    float scaleX = 0;
    float scaleY = 0;
    SDL_GetWindowSize(ctx->window, &ctx->windowWidth, &ctx->windowHeight);
    scaleX = (float)ctx->windowWidth / (float)pFrame->width;
    scaleY = (float)ctx->windowHeight / (float)(pFrame->width + pFrame->height);
//    scaleY = (float)rendY/(float)ctx->windowHeight/2;
    SDL_RenderClear(ctx->renderer);

    // Update SDL texture for the visualization
    if((res = update_visualization_widget(ctx, visWidgetCtx, texture, scaleY, scaleX) != 0)){
        fprintf(stderr,"Updating visualization failed\n");
        return res;
    }
    if((res = update_video_widget(ctx, videoWidgetCtx, pFrame) !=0)){
        fprintf(stderr,"Updating video failed\n");
        return res;
    };

    // Display to screen
    SDL_RenderPresent(ctx->renderer);
    return 0;
}