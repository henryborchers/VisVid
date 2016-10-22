//
// Created by henry on 10/11/2016.
//

#include <stdint.h>
#include <stdlib.h>
#include "visView.h"

visView *CreateVisView(int width, int height) {
    visView *new_visView = NULL;
    new_visView = (visView*)malloc(sizeof(visView));
    if(new_visView == NULL){
        return NULL;

    }
    new_visView->height = height;
    new_visView->width = width;

    new_visView->data = calloc((size_t)(width * height), sizeof(PixelValue));
    return new_visView;
}

void DestroyVisView(visView **pvd) {
    free((*pvd)->data);
    (*pvd)->data = NULL;
    (*pvd)->height = -1;
    (*pvd)->width = -1;
    free(*pvd);
    *pvd = NULL;
}

// TODO visViewUpdate
int visViewUpdate(visView *pD, visBuffer *buffer){
    size_t x;
    int y;
    int rc = 0;
    size_t buffersize = visBufferLength(buffer);
    PixelValue slice[buffersize];

    for(x = 0; x < buffersize; x++){
        for(y=0; y < pD->width; y++){
            rc = getResult(slice, buffer, x);
            if( rc != 0){
                return rc;
            };
            pD->data[x*y] = slice[y];
        }
    }
    return 0;
}

