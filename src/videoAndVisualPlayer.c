//
// Created by Borchers, Henry Samuel on 12/3/16.
//

#include <stdbool.h>
#include <visBuffer.h>
#include <visFrame.h>
#include <visVisualization.h>
#include <visView.h>
#include "videoDecoder.h"
#include "runPlayers.h"
#include "ramp.h"
#include "ffmpeg_converter.h"


static int vidVis_init();
static void vidVis_ctx_init(VidVisContext *ctx);
static int vidVis_build_window(VidVisContext *ctx, int videoWidth, int videoHeight);
static void vidVis_destroy_window(VidVisContext *ctx);
static int vidVis_refresh(VidVisContext *ctx, AVFrame *pFrame, visImageRGB *texture);
static void vidVis_cleanup();
static int variable_color(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);
static bool quitCalled(SDL_Event *pEvent);
static float min(float d, float d1);
static float enforce_range(float min, float max, float value);
static int generate_fakeresult(visVisualResult *pResult);

static uint8_t offset = 255;
static float offset_color_midtones = 1;
static float offset_color_highlights = 1;

int playVideoVis(const char *filename) {
    DecoderContext      *decoderCtx = NULL;
    AVFrame             *frame = NULL;
    visProcessContext   processContext;
    visView             *view = NULL;
    VisYUVFrame         *frameBuffer = NULL;
    visBuffer           *buffer = NULL;
    VidVisContext       ctx;
    SDL_Event           event;
    int                 res;
    visImageRGB         visualization;
    visVisualResult     result;

    int                 returncode = 0;


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


    // build buffer with the size of the frame
    puts("Creating buffer");
    if((buffer = VisBuffer_Create((size_t)videoWidth)) == NULL){
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
    if((res = VisVisualResult_Init(&result)) != 0 ) {
        fprintf(stderr, "VisVisualResult_Init Failed with code %d\n", res);
        return res;
    }

    //             Configure the size of the result data set
    if((res = VisVisualResult_SetSize(&result, videoWidth)) != 0){
        fprintf(stderr, "VisVisualResult_SetSize Failed\n");
        return res;
    }



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
                    }
                    break;
            }

        }


        // Get a frame
        res = decoderContext_NextFrame(decoderCtx, &frame);

        // If no frame is retrieved, rewind the video
        if(NULL == frame){
            decoderContext_Rewind(decoderCtx);
            continue;
        }

        // If there is an error with decoding, stop!
        if(res < 0 || frame == NULL){
            returncode = res;
            break;
        }

        // Convert the ffmpeg frame into a frame that can be analyzed
        if((res = ffmpeg2visframe(frameBuffer, frame)) != 0){
            fprintf(stderr, "ffmpeg2visframe failed with error code %d.\n", res);
            returncode = res;
            break;
        } else {

            // process the frame and save the data to a result data set
            if((res = visVisProcess(&result, frameBuffer, &processContext)) != 0){
                returncode = res;
                fprintf(stderr, "visVisProcess Failed\n");
                break;
            };

            // Add result data set to buffer
            if((res =  visBuffer_PushBackResult(buffer, &result)) != 0){
                returncode = res;
                fprintf(stderr, "visBuffer_PushBackResult Failed\n");
                break;
            };

            // update a view of the buffer
            if((res = visView_Update3(view, buffer)) != 0){
                returncode = res;
                fprintf(stderr, "visView_Update Failed with code %d.\n", res);
                break;
            }
//            // Render a picture of the view to the visualization image
            if((res = visViewRGB_GenerateRGBA(&visualization, view, visViewRGBA_value2color1)) != 0){
                returncode = res;
                fprintf(stderr, "visViewRGB_GenerateRGBA Failed with code %d.\n", res);
                break;
            }
        }


        vidVis_refresh(&ctx, frame, &visualization);
    }
    stopmainloop:

    puts("Destroying Frame buffer");
    VisYUVFrame_Destroy(&frameBuffer);

    puts("Destroying buffer");
    VisBuffer_Destroy(&buffer);

    puts("Destroying Window");
    vidVis_destroy_window(&ctx);

    puts("Destroying view");
    VisView_Destroy(&view);

    vidVis_cleanup();
    return returncode;
}

int generate_fakeresult(visVisualResult *pResult) {
    static uint8_t value = 0;
    int length = 0;
    VisVisualResult_GetSize(&length, pResult);
    PixelValue data[length];


    for (int i = 0; i < length; ++i) {
//        data[i] = value;
        int location = length - i - 1;
        uint8_t n_value = ((float)i/length * 256) + value;
        data[location] = n_value;
    }
    VisVisualResult_SetData(pResult, data, length);
    value++;
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

    // Calculate how large the video player should be when scaled based on the window
    videoWidget.x = 0;
    videoWidget.y = 0;
    videoWidget.h = (int)(videoOrig.h * scaleY);
    videoWidget.w = (int)(videoOrig.w * scaleX);

    // Calculate how large the visualization should be when scaled based on the window
    visualizationWidget.x = (int)(visualizationOrig.x * scaleX);
    visualizationWidget.y = (int)(visualizationOrig.y * scaleY);
    visualizationWidget.h = (int)(visualizationOrig.h * scaleY);
    visualizationWidget.w = (int)(visualizationOrig.w * scaleX);

    SDL_RenderClear(ctx->renderer);

    // Update SDL texture for the video frame
    if((res = SDL_UpdateYUVTexture(ctx->video.texture, &videoOrig,
                                   pFrame->data[0], pFrame->linesize[0],
                                   pFrame->data[1], pFrame->linesize[1],
                                   pFrame->data[2], pFrame->linesize[2])) != 0){
        fprintf(stderr, "%s\n", SDL_GetError());
        return res;
    };

    // Update SDL texture for the visualization
    SDL_UpdateTexture(ctx->visualization.texture, NULL, texture->plane, texture->pitch);

    // Draw the visualization
    SDL_RenderCopy(ctx->renderer, ctx->visualization.texture, NULL, &visualizationWidget);

    // Draw the video
    SDL_RenderCopy(ctx->renderer, ctx->video.texture, &videoOrig, &videoWidget);

    // Display to screen
    SDL_RenderPresent(ctx->renderer);
    return 0;
}


float min(float d, float d1) {

    return (d < d1) ? d: d1;
}
float enforce_range(float min, float max, float value) {
    if(value >= min && value <= max){
        return value;
    } else{
        if(value >= max){
            return max;
        }

        if(value <= min){
            return min;
        }
    }
    return value;
}

int variable_color(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {
    // FIXME: REPLACE WITH ORIGINAL FUNCTION
    // This is temp code that was modified from a web tutorial. Replace this with an original callback function.
    float i = (float)value / (float)255;
    const int NUM_COLORS = 4;
//    static float const STARTING_COLORS[NUM_COLORS][3] = { {0,0,0}, {0.2,0.2,0.3}, {0.7,0.7,0.5}, {1,1,1} };
//    static float const STARTING_COLORS[NUM_COLORS][3] = { {0,0,0}, {0.0,0.5,0.5}, {0.9,0.6,0.6}, {1,1,1} };
    static float const STARTING_COLORS[NUM_COLORS][3] = { {0,0,0}, {0.5,0.5,0.5}, {0.9,0.9,0.9}, {1,1,1} };
//    static float const STARTING_COLORS[NUM_COLORS][3] = { {0,0,0}, {1,0.2,0.2}, {1,1,0.2}, {1,1,1} };
    static float color[NUM_COLORS][3];
//    static float color[NUM_COLORS][3] = { {0,0,0}, {1,0.2,0.2}, {1,1,0.2}, {1,1,1} };

    color[1][0] = STARTING_COLORS[1][0] * offset_color_midtones;
    color[1][1] = STARTING_COLORS[1][1] * offset_color_midtones;
    color[1][2] = STARTING_COLORS[1][2] * offset_color_midtones;

    color[2][0] = (color[1][0] + STARTING_COLORS[2][0]) * offset_color_highlights;
    color[2][1] = (color[1][1] + STARTING_COLORS[2][1]) * offset_color_highlights;
    color[2][2] = (color[1][2] + STARTING_COLORS[2][2]) * offset_color_highlights;

    // A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.

    int idx1;        // |-- Our desired color will be between these two indexes in "color".
    int idx2;        // |
    float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.

    if(i <= 0)      {  idx1 = idx2 = 0;            }    // accounts for an input <=0
    else if(i>= 1)  {  idx1 = idx2 = NUM_COLORS-1; }    // accounts for an input >=0
    else
    {
        i = i * (NUM_COLORS-1);        // Will multiply value by 3.
        idx1  = (int)floor(i);                  // Our desired color will be after this index.
        idx2  = idx1+1;                        // ... and before this index (inclusive).
        fractBetween = i - (float)(idx1);    // Distance between the two indexes (0-1).

    }

    *r = (uint8_t)(value * min(1.0f, (color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0]));
    *g = (uint8_t)(value * min(1.0f, (color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1]));
    *b = (uint8_t)(value * min(1.0f, (color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2]));
    *a = value;
    return 0;
}