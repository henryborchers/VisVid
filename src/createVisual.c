//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <stdio.h>
#include "visVersion.h"
#include "runPlayers.h"
#ifdef NOC_FILE_DIALOG_IMPLEMENTATION
#include "noc/noc_file_dialog.h"
#endif


int main(int argc, char *argv[]){

    int ret;

    const char *filename = NULL;
    puts("Using VisVis library");
    printf("Version %s\n", VERSION_VIS_S);
    fflush(stdout);

    // Check args
    #ifdef NOC_FILE_DIALOG_IMPLEMENTATION
    if(argc == 1){
        puts("Opening up a file dialog box");
        filename = noc_file_dialog_open(NOC_FILE_DIALOG_OPEN, "mov\0*.mov\0mp4\0*.mp4\0mpg\0*.mpg\0mkv\0*.mkv\0", NULL, NULL);
        if(!filename){
            return 0;
        }
    }
    #else
    if(argc != 2){
        fprintf(stderr, "You need a file name to continue\n");
        return 1;
    }
    #endif

    if(argc == 2){
        filename = argv[1];
    }
    if((ret = playVideoVis(filename)) !=0){
        fprintf(stderr, "program didn't run correctly\n");
    };


}



