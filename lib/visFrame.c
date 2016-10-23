//
// Created by Borchers, Henry Samuel on 6/25/16.
//

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include "visFrame.h"

//
//enum PIXELCOLORSPACE {
//    PIXELCOLORSPACE_YUV = 0,
//    PIXELCOLORSPACE_Y = 1
//};



/**
 * @brief A frame of video. Each pixel has it's sub-sampling de-structured so that each pixel has a luma and two chroma values.
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

int SetPixelValueYUV(PixelYUV *pixel, PixelValue y, PixelValue u, PixelValue v) {
    if(pixel == NULL){
        return EFAULT;
    }
    pixel->Y = y;
    pixel->U = u;
    pixel->V = v;
    return 0;
}

int GetPixelValueYUV(PixelYUV *pixel, PixelValue *y, PixelValue *u, PixelValue *v) {
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

int GetVisYUVFrameSizePos(VisYUVFrame *frame, int64_t *pos) {
    *pos = frame->pos;
    return 0;
}

int visFillYUVFrame(VisYUVFrame *frame, visBrush *brush) {
    int x;
    int y;
    if(frame == NULL){
        return EFAULT;
    }

    for(x = 0; x < frame->width; x++){
        for(y = 0; y < frame->height; y++){
            PixelYUV *pixel = frame->data[y][x];
            pixel->Y = brush->Y;
            pixel->U = brush->U;
            pixel->V = brush->V;
        }
    }
    return 0;
}

int GetPixelFromYUVFrame(PixelYUV *pixel, VisYUVFrame *frame, int x, int y) {
    if(x > frame->width || y > frame->height){
        return EFAULT;
    }
    PixelYUV *pix = frame->data[y][x];
    pixel->U = pix->U;
    pixel->Y = pix->Y;
    pixel->V = pix->V;
    return 0;
}

int visDrawYUVPixel(VisYUVFrame *frame, visBrush *brush, int x, int y) {
    if(x > frame->width || y > frame->height){
        return EFAULT;
    }
    PixelYUV *pix = frame->data[y][x];
    pix->U = brush->U;
    pix->Y = brush->Y;
    pix->V = brush->V;
    return 0;
}

