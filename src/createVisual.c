//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <stdio.h>
#include "visVersion.h"
#include "runPlayers.h"
#include "filedialog.h"


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
        filename = getFilename();
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
        fprintf(stderr, "%s exited with error code: %d.\n", argv[0], ret);
        return ret;
    };
    return  0;

}



