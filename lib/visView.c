//
// Created by henry on 10/11/2016.
//

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
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

int visViewUpdate(visView *pView, visBuffer *buffer){
    size_t x;
    int y;
    int valid_result = 0;
    size_t buffersize = visBufferLength(buffer);
    PixelValue currentSlice[pView->height];
    PixelValue lastSlice[pView->height];

    for(x = 0; x < buffersize; x++){
        // If the result is null the result code will be 0
        valid_result = getResult(currentSlice, buffer, x);
        if( valid_result < 0){
            return valid_result;
        };
        for(y = 0; y < pView->width; y++){
            // If the result is a true result, the memory can be copied
            if(valid_result){
                pView->data[x + pView->width * y] = currentSlice[y];
            } else {
                // If the result isn't a true result (for example: there is no data calculated yet), do one of the following.
                // If it's the first one, render it out as black.
                if( x == 0){
                    pView->data[x + pView->width * y] = 0;
                    currentSlice[y] = 0;
                } else{
                    // Otherwise, use the last good slice
                    pView->data[x + pView->width * y] = lastSlice[y];
                }
            }
        }
        if(valid_result || x == 0){
            memcpy(lastSlice, currentSlice, pView->height);
        }
    }
    return 0;
}

int visViewRGBA(visImageRGB *result, visView *pView){

    // assert that neither pointer is NULL.

    if(NULL == result || NULL == pView){
        return EFAULT;
    }

    // Verify that the image is the same size as the view

    if(result->width != pView->width){
        return -1;
    }

    if(result->height != pView->height){
        return -1;
    }

    // Render the data

    for (int y = 0; y < result->height; ++y) {
        for (int x = 0; x < result->width; ++x) {
            PixelValue value = pView->data[x + pView->width * y];
            visImageWritePixelRGB(result, x,y, value, value, value, value);
        }
    }
    return 0;
}

