//
// Created by Borchers, Henry Samuel on 11/11/16.
//

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "visImageWriter.h"

int visImageRGB_Alloc(visImageRGB *t, int width, int height) {
//    pitch * size of the the pixel + padding
// TODO: Caculate padding for at least 4 bytes
    size_t pitch = (width * (sizeof(uint8_t) * 4));


    t->plane = malloc(pitch * height );
    if(NULL == t->plane){
        return ENOMEM;
    }
    t->pitch = (int)pitch;

    t->height = height;
    t->width = width;

    return 0;
}

void visImageRGB_FreeData(visImageRGB *t) {
    if(t->plane != NULL){
        free(t->plane);
        t->plane = NULL;
    }
    t->width = -1;
    t->height = -1;
    t->pitch = -1;


}

int visImageRGB_WritePixel(visImageRGB *t, int pixelX, int pixelY, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int y_offset = pixelY * t->pitch;
    int x_offset = (pixelX * sizeof(uint8_t) * 4);
    int offset = y_offset + x_offset;

    memcpy(&t->plane[offset], ((const uint8_t []){a,b,g,r}), sizeof(uint8_t) * 4);
    return 0;
}