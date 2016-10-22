//
// Created by henry on 10/11/2016.
//

#include <stdint.h>
#include <stdlib.h>
#include "visDisplay.h"

visDisplay *CreateVisDisplay(int width, int height) {
    visDisplay *new_visDisplay = NULL;
    new_visDisplay = (visDisplay*)malloc(sizeof(visDisplay));
    if(new_visDisplay == NULL){
        return NULL;

    }
    new_visDisplay->height = height;
    new_visDisplay->width = width;

    new_visDisplay->data = calloc((size_t)(width * height), sizeof(PixelValue));
    return new_visDisplay;
}

void DestroyVisDisplay(visDisplay **pvd) {
    free((*pvd)->data);
    (*pvd)->data = NULL;
    (*pvd)->height = -1;
    (*pvd)->width = -1;
    free(*pvd);
    *pvd = NULL;
}

// TODO visDisplayUpdate
int visDisplayUpdate(visDisplay *pD, visBuffer *buffer){
    int x;
    int y;
    size_t buffersize = visBufferLength(buffer);

    for(x = 0; x < buffersize; x++){
        for(y=0; y < pD->width; y++){
            // todo Change this line to make sense
            pD->data[x*y] = 0;
        }
    }
    return 0;
}

