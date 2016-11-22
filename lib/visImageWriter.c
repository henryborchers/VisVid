//
// Created by Borchers, Henry Samuel on 11/11/16.
//

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "visImageWriter.h"

int allocImageRGB(VisImageRGB *t, int height, int width) {
    t->plane = malloc(sizeof(uint8_t) * t->pitch * height * 4);
    if(NULL == t->plane){
        return ENOMEM;
    }

    t->height = height;
    t->width = width;

    t->pitch = width * 4;
    return 0;
}

void freeImageRGB(VisImageRGB *t) {
    if(t->plane != NULL){
        free(t->plane);
        t->plane= NULL;
    }


}

int writePixelRGB(VisImageRGB *t, int pixelY, int pixelX, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int y_offset = pixelY * t->pitch;
    int x_offset = (pixelX * sizeof(uint8_t) * 4);
    int offset = y_offset + x_offset;

    memcpy(&t->plane[offset], ((const uint8_t []){a,b,g,r}), sizeof(uint8_t) * 4);
    return 0;
}