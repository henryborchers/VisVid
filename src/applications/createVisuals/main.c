//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include "videoAndVisualPlayer.h"
#include "filedialog.h"
#include "visvid/visVersion.h"
#include <stdio.h>

static const char *get_file(int argc, char *argv[]);

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

    DisplayWidgetContext       visualizationCtx;

    puts("Initializing context");
    vidVis_ctx_init2(&visualizationCtx, decoderCtx);
    if((ret = vidVis_open_window(&visualizationCtx)) != 0){
        fprintf(stderr, "Building Window failed with error code %d\n ", ret);
    }
    
    if(ret == 0){
        if ((ret = playVideoVis(decoderCtx, &visualizationCtx)) != 0) {
            fprintf(stderr, "%s exited with error code: %d.\n", argv[0], ret);
        };
    }
    puts("Destroying Window");
    vidVis_destroy_window(&visualizationCtx);

    puts("Destroying decoder Context");
    decoderContext_Destroy(&decoderCtx);

    vidVis_cleanup();
    return ret;

}
