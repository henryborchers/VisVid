//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include "videoAndVisualPlayer.h"
#include "filedialog.h"
#include "visvid/visVersion.h"
#include <stdio.h>

static const char *get_file(int argc, char *argv[]);

void vidVis_destroy_widgets(VidVisWidget *visualizationWidget, VidVisWidget *videoWidget);

#ifdef NOC_FILE_DIALOG_IMPLEMENTATION

const char *get_file(int argc, char *argv[]) {

    if (argc == 1) {
        puts("Opening up a file dialog box");
        return getFilename();
    }
    if (argc == 2) {
        return argv[1];
    }
    return NULL;
}

#else
const char *get_file(int argc, char *argv[]) {
    if (argc == 2) {
            return argv[1];
        } else {
            return NULL;
            }
}
#endif


int main(int argc, char *argv[]) {

    int ret = 0;

    const char *filename = NULL;
    puts("Using VisVis library");
    printf("Version %s\n", VERSION_VIS_S);
    fflush(stdout);

    // Check args
#ifdef NOC_FILE_DIALOG_IMPLEMENTATION
    if (argc == 1) {
        filename = get_file(argc, argv);
        if (!filename) {
            return 0;
        }
    }
#else
    if(argc != 2){
        fprintf(stderr, "You need a file name to continue\n");
        return 1;
    }
#endif

    if (argc == 2) {
        filename = argv[1];
    }
    playVideoVisInit();

    DecoderContext *decoderCtx = decoderContext_Create(filename);
    if(decoderCtx == NULL){
        fprintf(stderr, "Unable to load file \"%s\"", filename);
        return 1;
    } else{
        puts("Video Opened");
    }
    VidVisWidget visualizationWidget;
    VidVisWidget videoWidget;
    DisplayWidgetContext       visualizationWindowCtx;

    puts("Initializing context");
    vidVis_ctx_init(&visualizationWindowCtx, &visualizationWidget, &videoWidget, decoderCtx);
    SDL_ShowWindow(visualizationWindowCtx.window);

    if ((ret = playVideoVis(decoderCtx, &visualizationWindowCtx, &visualizationWidget, &videoWidget)) != 0) {
        fprintf(stderr, "%s exited with error code: %d.\n", argv[0], ret);
    };
    puts("Destroying Window");

    vidVis_destroy_widgets(&visualizationWidget, &videoWidget);
    vidVis_destroy_window(&visualizationWindowCtx);

    puts("Destroying decoder Context");
    decoderContext_Destroy(&decoderCtx);

    vidVis_cleanup();
    return ret;

}

void vidVis_destroy_widgets(VidVisWidget *visualizationWidget, VidVisWidget *videoWidget) {
    puts("Destroying widgets");
    if(NULL != videoWidget->texture){
        SDL_DestroyTexture(videoWidget->texture);
    }
    videoWidget->height = -1;
    videoWidget->width = -1;
    videoWidget->x = -1;
    videoWidget->y = -1;

    if(NULL != visualizationWidget->texture) {
        SDL_DestroyTexture(visualizationWidget->texture);
    }
    visualizationWidget->height = -1;
    visualizationWidget->width = -1;
    visualizationWidget->x = -1;
    visualizationWidget->y = -1;
}
