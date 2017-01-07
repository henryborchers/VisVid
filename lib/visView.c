//
// Created by henry on 10/11/2016.
//

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <stdio.h>
#include "visView.h"


visView *VisView_Create(int width, int height) {
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

void VisView_Destroy(visView **pvd) {
    free((*pvd)->data);
    (*pvd)->data = NULL;
    (*pvd)->height = -1;
    (*pvd)->width = -1;
    free(*pvd);
    *pvd = NULL;
}


int visView_Update2(visView *pView, visBuffer *buffer) {
    int x;
    int y;
    int res = 0;

    PixelValue currentSlice[pView->width];


    for(x = 0; x < pView->width; x++){
        res = visBuffer_getResult(currentSlice, buffer, x);
        if(res == 0){

            for (y = 0; y < pView->height; ++y) {
                pView->data[x + pView->width * y] = currentSlice[y];

            }
        } else {
            for (y = 0; y < pView->height; ++y) {

                pView->data[x + pView->width * y] = 0;

            }
        }


    }
    return 0;
}

int visView_Update3(visView *pView, visBuffer *buffer) {
    int x;
    int y;
    int res = 0;

    PixelValue currentSlice[pView->width];


    for(x = 0; x < pView->width; x++){
        size_t length = visBuffer_getLength(buffer);
        res = visBuffer_getResult(currentSlice, buffer, length - x);
        if(res == 0){

            for (y = 0; y < pView->height; ++y) {
                pView->data[pView->width - x + pView->width * y] = currentSlice[y];

            }
        } else {
            for (y = 0; y < pView->height; ++y) {

                pView->data[pView->width - x + pView->width * y] = 0;

            }
        }


    }
    return 0;
}



// FIXME visView_Update is broken
// depricated
int visView_Update(visView *pView, visBuffer *buffer){
    int x;
    int y;
    int valid_result = 0;
    size_t buffersize = visBuffer_getLength(buffer);
//    int starting = buffersize - pView->width;
//    if(starting < 0){
//        starting = 0;
//    }
//    int ending = starting + pView->width;

    PixelValue currentSlice[pView->height];
    PixelValue lastSlice[pView->height];

    for(x = 0; x < buffersize; x++){
        // If the result is null the result code will be 0
        valid_result = visBuffer_getResult(currentSlice, buffer, x);
//        printf("value of x is %d\n", x);
        if( valid_result < 0){

            fprintf(stderr, "Invalid result\n");
            return valid_result;
        };
        for(y = 0; y < pView->width; y++){
            // If the result is a true result, the memory can be copied
            if(valid_result == 1){
                pView->data[x + pView->width * y] = currentSlice[y];
            } else {
//                fprintf(stderr, "Invalid data\n");
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



int visViewRGB_GenerateRGBA(visImageRGB *out, visView *pView,
                             int (*callback)(PixelValue, uint8_t *, uint8_t *, uint8_t *, uint8_t *)) {

    // assert that neither pointer is NULL.

    if(NULL == out || NULL == pView){
        return EFAULT;
    }

    // Verify that the image is the same size as the view

    if(out->width != pView->width){
        return -1;
    }

    if(out->height != pView->height){
        return -1;
    }

    // Render the data
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
    for (int y = 0; y < out->height; ++y) {
        for (int x = 0; x < out->width; ++x) {
            callback(pView->data[x + pView->width * y], &r, &g, &b, &a);
            visImageRGB_WritePixel(out, x, y, r, g, b, a);
        }
    }
    return 0;
}

int visViewRGBA_value2BW(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {
    *r = value;
    *g = value;
    *b = value;
    *a = value;
    return 0;
}


