//
// Created by Borchers, Henry Samuel on 6/25/16.
//

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include "visFrame.h"

enum PIXELCOLORSPACE {
    PIXELCOLORSPACE_YUV = 0,
    PIXELCOLORSPACE_Y = 1
};

/**
 * @struct PixelYUV
 * @brief The pxiel split into it's YUV components
 *
 * This represent a pixel's value. NOTE: While YUV is really an analog color format and the true format is YCrBr,
 * it's much easier to write YUV as a varible name and YCrBr is more thought of in terms of their analog counterpart.
 */
struct PixelYUV {
    uint8_t Y;    /**< Luma value for a pixel.*/
    uint8_t U;    /**< Cr chroma value for pixel.*/
    uint8_t V;    /**< Br chroma value for pixel.*/
};


/**
 * @brief A frame of video. Each pixel has it's subsampling destructured so that each pixel has a luma and two chroma values.
 */
struct VisYUVFrame{
    int height;                     /**< Number of rows of pixels in the frame.*/
    int width;                      /**< Number of columns of pixels in the frame.*/
    struct PixelYUV ***data;       /**< Pixels that make up the frame.*/
    int64_t pos;                    /**< timestamp for the frame.*/
} ;

void FreeFrameData(VisYUVFrame **pFrame);

PixelYUV *CreatePixelYUV() {
    PixelYUV *newPixel = NULL;
    newPixel = (PixelYUV*)malloc(sizeof(PixelYUV));
    if(newPixel == NULL){
        return NULL;
    }
    newPixel->Y = 0;
    newPixel->U = 0;
    newPixel->V = 0;
    return newPixel;
}

void DestroyPixelYUV(PixelYUV **pixel) {
    free(*pixel);
    (*pixel) = NULL;
}

int SetPixelYUV(PixelYUV *pixel, uint8_t y, uint8_t u, uint8_t v) {
    if(pixel == NULL){
        return EFAULT;
    }
    pixel->Y = y;
    pixel->U = u;
    pixel->V = v;
    return 0;
}

int GetPixelYUV(PixelYUV *pixel, uint8_t *y, uint8_t *u, uint8_t *v) {
    if(pixel == NULL){
        return EFAULT;
    }
    *y = pixel->Y;
    *u = pixel->U;
    *v = pixel->V;
    return 0;
}


VisYUVFrame *CreateVisYUVFrame() {
    VisYUVFrame *newFrame = NULL;
    newFrame = (VisYUVFrame*)malloc(sizeof(VisYUVFrame));
    if(newFrame == NULL){
        return NULL;
    }
    newFrame->height = -1;
    newFrame->width = -1;
    newFrame->data = NULL;
    newFrame->pos = -1;

    return newFrame;
}

void DestroyVisYUVFrame(VisYUVFrame **frame) {
    FreeFrameData(frame);
    (*frame)->pos = -1;
    (*frame)->height = -1;
    (*frame)->width = -1;
    (*frame)->data = NULL;
    (*frame) = NULL;

}

void FreeFrameData(VisYUVFrame **pFrame) {
    int y,x;
    if((*pFrame)->data != NULL){
        for(y = 0; y < (*pFrame)->height; y++){
            for(x = 0; x < (*pFrame)->width; x++){
                PixelYUV *p =(*pFrame)->data[y][x];
                if(p != NULL){
                    DestroyPixelYUV(&p);
                }

            }
            free((*pFrame)->data[y]);
        }
    free((*pFrame)->data);
    }

}

int GetVisYUVFrameSize(VisYUVFrame *frame, int *width, int *height) {
    if(frame == NULL){
        return EFAULT;
    }
    *width = frame->width;
    *height = frame->height;
    return 0;
}

int SetVisYUVFrameSize(VisYUVFrame *frame, int width, int height) {
    int y, x;

    if(frame == NULL){
        return EFAULT;
    }
    // Free any frame data first
    FreeFrameData(&frame);



    // Create new data
    frame->data = (PixelYUV ***) malloc(sizeof(PixelYUV **) * height);

    for(y = 0; y < height; y++){
        frame->data[y] = (PixelYUV **) malloc(sizeof(PixelYUV *) * width);
        for(x = 0; x < width; x++){
            frame->data[y][x] = CreatePixelYUV();
        }
    }

    frame->height = height;
    frame->width = width;

    return 0;
}

