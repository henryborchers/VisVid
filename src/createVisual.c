//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <stdio.h>
#include "visVersion.h"
#include "runPlayers.h"




int main(int argc, char *argv[]){

    int ret;


    puts("Using VisVis library");
    printf("Version %s\n", VERSION_VIS_S);
    fflush(stdout);

    // Check args
    if(argc != 2){
        fprintf(stderr, "You need a file name to continue\n");
        return 1;
    }

    if((ret = playVideoVis(argv[1])) !=0){
        fprintf(stderr, "program didn't run correctly\n");
    };


}



