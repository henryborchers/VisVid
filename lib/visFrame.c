//
// Created by Borchers, Henry Samuel on 6/25/16.
//

#include <stdint.h>
#include <MacTypes.h>
#include <stdlib.h>
#include "visFrame.h"

enum PIXELCOLORSPACE {
    PIXELCOLORSPACE_YUV = 0,
    PIXELCOLORSPACE_Y = 1
};

/**
 * @brief The pxiel split into it's YUV components
 *
 * This represent a pixel's value. NOTE: While YUV is really an analog color format and the true format is YCrBr,
 * it's much easier to write YUV as a varible name and YCrBr is more thought of in terms of their analog counterpart.
 */
struct _PixelYUV {
    uint8_t Y;    /** Luma value for a pixel*/
    uint8_t U;    /** Cr chroma value for pixel*/
    uint8_t V;    /** Br chroma value for pixel*/
};


/**
 * @brief A frame of video. Each pixel has it's subsampling destructured so that each pixel has a luma and two chroma values.
 */
struct _VisYUVFrame{
    int height;                     /** Number of rows of pixels in the frame*/
    int width;                      /** Number of columns of pixels in the frame*/
    struct _PixelYUV ***data;       /** Pixels that make up the frame*/
    int64_t pos;                    /** timestamp for the frame */
} ;

PixelYUV *CreatePixelYUV() {
    PixelYUV *newPixel = NULL;
    newPixel = (PixelYUV*)malloc(sizeof(PixelYUV));
    newPixel->Y = 0;
    newPixel->U = 0;
    newPixel->V = 0;
    return newPixel;
}

void DestroyPixelYUV(PixelYUV **pixel) {
    free(*pixel);
    (*pixel) = NULL;
}

int setPixelYUV(PixelYUV *pixel, uint8_t y, uint8_t u, uint8_t v) {
    pixel->Y = y;
    pixel->U = u;
    pixel->V = v;
    return 0;
}

void getPixelYUV(PixelYUV *pixel, uint8_t *y, uint8_t *u, uint8_t *v) {
    *y = pixel->Y;
    *u = pixel->U;
    *v = pixel->V;
}

