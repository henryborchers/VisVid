//
// Created by Borchers, Henry Samuel on 11/11/16.
//

#include "visvid/visvid.h"
#include "visImageWriter.h"
#include "visvid/utils.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

const int ALPHA_OFFSET = 0;
const int BLUE_OFFSET = sizeof(uint8_t) * 1;
const int GREEN_OFFSET = sizeof(uint8_t) * 2;
const int RED_OFFSET = sizeof(uint8_t) * 3;
static int calculate_padding(int offset, int align);

int calculate_padding(int offset, int align) {
//    return (align - (offset & (align - 1))) & (align - 1);
     return (-offset) & (align - 1);
}

int visImageRGB_Alloc(visImageRGB *t, int width, int height) {
//    pitch * size of the the pixel + padding
//    size_t pitch = (width * (sizeof(uint8_t) * 4));

//    int align = 4;
//    int offset = width;
//    t->plane = calloc(pitch, height );

    int padding = calculate_padding(width * sizeof(uint8_t) * 4, 4);

    size_t pitch = (width + (size_t)padding) * sizeof(uint8_t) * 4;
    t->plane = malloc(pitch * height);
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

int visImageRGB_readPixel(const visImageRGB *t, int pixelX, int pixelY, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {


    int y_offset = pixelY * t->pitch;
    int x_offset = (pixelX * sizeof(uint8_t) * 4);
    int offset = y_offset + x_offset;

    if(a){
        *a = t->plane[offset + ALPHA_OFFSET];
    }

    if(r){
        *r = t->plane[offset + RED_OFFSET];
    }

    if(g){
        *g = t->plane[offset + GREEN_OFFSET];

    }

    if(b){
    *b = t->plane[offset + BLUE_OFFSET];
    }


    return 0;
}

//TODO: Create a memory tests for visImage allocation and deallocation
int visImage_Alloc(visImage *t, int width, int height, int components_per_pixel) {
    int padding = calculate_padding(width * components_per_pixel * sizeof(uint8_t), 4);

    t->height = height;
    t->width = width;
    t->num_pix_components = components_per_pixel;
    t->pitch = (width + (size_t)padding) * sizeof(uint8_t);
    t->plane = malloc(sizeof(PixelValue) * height * t->pitch);
    return 0;
}
void visImage_FreeData(visImage *pImage) {
    pImage->height = -1;
    pImage->width = -1;
    pImage->num_pix_components = -1;
    pImage->pitch = -1;
    free(pImage->plane);
    pImage->plane = NULL;

}