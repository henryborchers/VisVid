//
// Created by Borchers, Henry Samuel on 12/3/16.
//

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_log.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <libavutil/frame.h>
#include <math.h>
#include <stdio.h>
#include <visvid/visvid.h>
#include "ffmpeg_converter.h"
#include "videoAndVisualPlayer.h"
#include "videoDecoder.h"
#define NUM_COLORS 4

static int vidVis_init();

static int setup_visualization_widget(VidVisWidget *visCtx, const DisplayWidgetContext *ctx, const DecoderContext *decoderCtx);
static int setup_video_widget(VidVisWidget *videoCtx, const DisplayWidgetContext *ctx, const DecoderContext *decoderCtx);
static int setup_window(DisplayWidgetContext *windowCtx, const DecoderContext *decoderCtx, const char* title);
//static int vidVis_refresh_widgets(DisplayWidgetContext *ctx, VidVisWidget *visWidgetCtx, VidVisWidget *videoWidgetCtx,
//                                  const SDL_Rect *videoWidget, const SDL_Rect *visualizationWidget);

static void toggle_fullscreen(SDL_Window *pWindow, VidVisWidget *visWidget, VidVisWidget *videoWidget);
static int getCombinedWindowSize(int *w, int *h, const VidVisWidget *visWidget, const VidVisWidget *videoWidget);

static int write_visualization_texture(VidVisWidget *widget, visImageRGB *texture);

static int render_widget(SDL_Renderer *pRenderer, const VidVisWidget *widget, int w, int h, int x, int y);

int write_video_texture(VidVisWidget *widget, const AVFrame *pFrame);

static SDL_Rect getVideoCoordinateStretch(AVFrame *pFrame, int windowHeight, int windowWidth, int x, int y);

static SDL_Rect getVisualizationCoordinateStretch(VidVisWidget *widget, const AVFrame *frame, int windowHeight, int windowWidth, int x, int y);

typedef struct RenderQueueItem{
    const VidVisWidget *widget;
    const SDL_Rect *coordinates;

} RenderQueueItem;

static int render_widgets(SDL_Renderer *pRenderer, const struct RenderQueueItem *items, int count);

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
    int windowHeight, windowWidth;
    Uint32 startframeTime = SDL_GetTicks();
    PixelValue *pixelBuffer =(PixelValue*) malloc(sizeof(int) * view->width);
    while(1){
        while(SDL_PollEvent(&event) != 0){
            switch(event.type){
                case SDL_QUIT:
                    goto stopmainloop;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_q:
                            puts("quit");
                            goto stopmainloop;
                        default:break;
                    }
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.clicks == 2){
                        toggle_fullscreen(vidCtx->window, visWidget, videoWidget);
                    }
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event){
                        case SDL_WINDOWEVENT_RESIZED:

                            SDL_GetWindowSize(vidCtx->window, &windowHeight, &windowWidth);
                            printf("Resized to %d:%d\n", windowHeight, windowWidth);
//                            TODO: Resize the render
                            break;
                    }
                    break;
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


            Uint32 frameTime = SDL_GetTicks() - startframeTime;
            if (frameTime > 50){
                // update a view of the buffer
//                PixelValue *pixelBuffer =(PixelValue*) malloc(sizeof(int) * view->width);
//                visView_Update5(view, buffer,)
                if((rc = visView_Update5(view, buffer, pixelBuffer, view->width)) != 0){
//                if((rc = visView_Update4(view, buffer)) != 0){
                    returncode = rc;
                    fprintf(stderr, "visView_Update Failed with code %d.\n", rc);
                    break;
                }
//                free(pixelBuffer);
//            // Render a picture of the view to the visualization image
                if((rc = visViewRGB_GenerateRGBA(&vidCtx->buffer, view, visViewRGBA_value2color1)) != 0){
                    returncode = rc;
                    fprintf(stderr, "visViewRGB_GenerateRGBA Failed with code %d.\n", rc);
                    break;
                }
                int windowWidth, windowHeight;
                SDL_GetWindowSize(vidCtx->window, &windowWidth, &windowHeight);

                int res;
    //        Write textures for the widgets
                if((res = write_visualization_texture(visWidget, &vidCtx->buffer) != 0)){
                    fprintf(stderr,"write_visualization_texture failed\n");
                    return res;
                }

                if((res = write_video_texture(videoWidget, frame))){
                    fprintf(stderr,"write_video_texture() failed\n");
                    return res;
                }

    //        Draw widgets to the screen
                const SDL_Rect video_coordinates = getVideoCoordinateStretch(frame, windowHeight, windowWidth, 0, 0);
                const SDL_Rect visualization_coordinates = getVisualizationCoordinateStretch(videoWidget,
                                                                                             frame,
                                                                                             windowHeight,
                                                                                             windowWidth,
                                                                                             0,
                                                                                             video_coordinates.h);
                RenderQueueItem widgets[] ={
                        {visWidget, &visualization_coordinates},
                        {videoWidget, &video_coordinates},
                };

                if((res = render_widgets(
                        vidCtx->renderer, widgets,
                        sizeof(widgets) / sizeof(RenderQueueItem)))
                        ){
                    fprintf(stderr,"render_widgets() failed\n");
                    return res;
                }
                startframeTime = SDL_GetTicks();
                SDL_RenderPresent(vidCtx->renderer);
            }
        }


//        TODO present only if interval is above a certain number
    }
    free(pixelBuffer);
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

SDL_Rect
getVisualizationCoordinateStretch(VidVisWidget *widget, const AVFrame *frame, int windowHeight, int windowWidth, int x,
                                  int y) {
    float scaleX = (float)windowWidth  / (float)frame->width;
    float scaleY = (float)windowHeight / (float)(frame->width + frame->height);

    SDL_Rect result;
    result.x = x;
    result.y = y;
    result.h = windowHeight - (int)round(((float)widget->height * scaleY));
    result.w = (int)(widget->width * scaleX);

    return result;
}

SDL_Rect getVideoCoordinateStretch(AVFrame *frame, int windowHeight, int windowWidth, int x, int y) {
    float scaleX = (float)windowWidth / (float)frame->width;
    float scaleY = (float)windowHeight / (float)(frame->width + frame->height);
    SDL_Rect result;
    result.w = (int)(((float)frame->width * scaleX));
    result.h = (int)round(((float)frame->height) * scaleY);
    result.x = x;
    result.y = y;
    return result;
}

int getCombinedWindowSize(int *w, int *h, const VidVisWidget *visWidget, const VidVisWidget *videoWidget){
    if(w == NULL || h == NULL || visWidget == NULL || videoWidget == NULL){
        return -1;
    }
    *w = visWidget->width;
    *h = videoWidget->height + visWidget->height;
    return 0;
}
void toggle_fullscreen(SDL_Window *pWindow, VidVisWidget *visWidget, VidVisWidget *videoWidget) {
    if( (Uint32)SDL_WINDOW_FULLSCREEN_DESKTOP & SDL_GetWindowFlags(pWindow)){
        SDL_SetWindowFullscreen(pWindow, 0);
        int w;
        int h;
        if(getCombinedWindowSize(&w, &h, visWidget, videoWidget) != 0){
            fputs("getCombinedWindowSize failed\n", stderr);
            return;
        }
        SDL_SetWindowSize(pWindow, w, h);
    } else{
        SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
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

    int windowWidth = videoWidth;
    int windowHeight = (videoWidth + videoHeight);
    windowCtx->window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            windowWidth,
            windowHeight,
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


int write_visualization_texture(VidVisWidget *widget, visImageRGB *texture) {
    SDL_Rect visualizationOrig;
    visualizationOrig.x = widget->x;
    visualizationOrig.y = widget->y;
    visualizationOrig.w = widget->width;
    visualizationOrig.h = widget->height;
    SDL_UpdateTexture(widget->texture, &visualizationOrig, texture->plane, texture->pitch);
    return 0;

}

//int update_visualization_widget(const DisplayWidgetContext *ctx, VidVisWidget *widget, visImageRGB *texture, float scaleY,
//                                float scaleX) {
//    int windowHeight;
//    int windowWidth;
//    SDL_GetWindowSize(ctx->window, &windowWidth, &windowHeight);
//
//    SDL_Rect visualizationOrig;
//    SDL_Rect visualizationWidget;
//
//    visualizationOrig.x = widget->x;
//    visualizationOrig.y = widget->y;
//    visualizationOrig.w = widget->width;
//    visualizationOrig.h = widget->height;
//
//    // Calculate how large the visualization should be when scaled based on the window
//    visualizationWidget.x = (int)(visualizationOrig.x * scaleX);
//    visualizationWidget.h = windowHeight-(int)round(((float)visualizationOrig.h * scaleY));
//    visualizationWidget.y = windowHeight - (widget->height* scaleY);
//    visualizationWidget.w = (int)(widget->width * scaleX);
//
//    SDL_UpdateTexture(widget->texture, &visualizationOrig , texture->plane, texture->pitch);
//    // Draw the visualization
//
//    SDL_RenderCopy(ctx->renderer, widget->texture, &visualizationOrig, &visualizationWidget);
//    return 0;
//}


int write_video_texture(VidVisWidget *widget, const AVFrame *pFrame) {
    SDL_Rect videoOrig;

    videoOrig.x = widget->x;
    videoOrig.y = widget->y;
    videoOrig.w = pFrame->width;
    videoOrig.h = pFrame->height;

    int res = 0;
    if((res = SDL_UpdateYUVTexture(widget->texture, &videoOrig,
                                   pFrame->data[0], pFrame->linesize[0],
                                   pFrame->data[1], pFrame->linesize[1],
                                   pFrame->data[2], pFrame->linesize[2])) != 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return res;
    }
    return 0;
}


int render_widget(SDL_Renderer *pRenderer, const VidVisWidget *widget, int w, int h, int x, int y) {
    SDL_Rect videoOrig;
    videoOrig.x = widget->x;
    videoOrig.y = widget->y;
    videoOrig.h = widget->height;
    videoOrig.w = widget->width;

    SDL_Rect videoWidget;
    videoWidget.x = x;
    videoWidget.y = y;
    videoWidget.h = h;
    videoWidget.w = w;

    SDL_RenderCopy(pRenderer, widget->texture, &videoOrig, &videoWidget);
    return 0;
}

int render_widgets(SDL_Renderer *pRenderer, const struct RenderQueueItem *items, int count) {
        for (int i = 0; i < count; ++i) {
            RenderQueueItem item = items[i];
            if (item.widget == NULL || item.coordinates == NULL){
                return -1;
            }
            int res;
            if((res = render_widget(
                    pRenderer,
                    items[i].widget,
                    items[i].coordinates->w,
                    items[i].coordinates->h,
                    items[i].coordinates->x,
                    items[i].coordinates->y) != 0)){
                return res;
            }

    }
    return 0;
}

