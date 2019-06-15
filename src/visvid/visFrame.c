//
// Created by Borchers, Henry Samuel on 6/25/16.
//

#include "visvid/utils.h"
#include "visFrame.h"
#include <errno.h>
#include <stdlib.h>
//#include <stdint.h>

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
    struct PixelYUV *data;         /**< Pixels that make up the frame.*/
    int64_t pos;                    /**< timestamp for the frame.*/
} ;

void FreeFrameData(VisYUVFrame **pFrame);

PixelYUV *PixelYUV_Create() {
    PixelYUV *newPixel = (PixelYUV*)malloc(sizeof(PixelYUV));
    if(newPixel == NULL){
        return NULL;
    }
    newPixel->Y = 0;
    newPixel->U = 0;
    newPixel->V = 0;
    return newPixel;
}

void PixelYUV_Destroy(PixelYUV **pixel) {
    PixelYUV  *p = *pixel;
    free(p);
    (*pixel) = NULL;
}

int PixelYUV_setValue(PixelYUV *pixel, PixelValue y, PixelValue u, PixelValue v) {
    if(pixel == NULL){
        return EFAULT;
    }
    pixel->Y = y;
    pixel->U = u;
    pixel->V = v;
    return 0;
}

int PixelYUV_getValue(const PixelYUV *pixel, PixelValue *y, PixelValue *u, PixelValue *v) {
    if(pixel == NULL){
        return EFAULT;
    }
    *y = pixel->Y;
    *u = pixel->U;
    *v = pixel->V;
    return 0;
}


VisYUVFrame *VisYUVFrame_Create() {
    VisYUVFrame *newFrame = (VisYUVFrame*)malloc(sizeof(VisYUVFrame));
    if(newFrame == NULL){
        return NULL;
    }
    newFrame->height = -1;
    newFrame->width = -1;
    newFrame->data = NULL;
    newFrame->pos = -1;

    return newFrame;
}

void VisYUVFrame_Destroy(VisYUVFrame **frame) {
//    FreeFrameData(frame);
    (*frame)->pos = -1;
    (*frame)->height = -1;
    (*frame)->width = -1;
    free((*frame)->data);
    (*frame)->data = NULL;
    free((*frame));
    (*frame) = NULL;

}

void FreeFrameData(VisYUVFrame **pFrame) {
    if((*pFrame)->data != NULL){
        free((*pFrame)->data);
    }

}

int VisYUVFrame_GetSize(const VisYUVFrame *frame, int *width, int *height) {
    if(frame == NULL){
        return EFAULT;
    }
    *width = frame->width;
    *height = frame->height;
    return 0;
}

int VisYUVFrame_SetSize(VisYUVFrame *frame, int width, int height) {

    if(frame == NULL){
        return EFAULT;
    }
    // Free any frame data first
    FreeFrameData(&frame);



    // Create new data
    frame->data = (PixelYUV *) malloc(sizeof(PixelYUV) * height * width);
    if(frame->data == NULL){
        return EFAULT;
    }


    frame->height = height;
    frame->width = width;

    return 0;
}

int VisYUVFrame_SetPos(VisYUVFrame *frame, int64_t *result) {
    *result = frame->pos;
    return 0;
}

int visYUVFrame_Fill(VisYUVFrame *frame, visBrush *brush) {
    int x;
    int y;
    if(frame == NULL){
        return EFAULT;
    }

    for(x = 0; x < frame->width; x++){
        for(y = 0; y < frame->height; y++){
            PixelYUV *pixel = &frame->data[x + frame->width * y];
            pixel->Y = brush->Y;
            pixel->U = brush->U;
            pixel->V = brush->V;
        }
    }
    return 0;
}

int VisYUVFrame_getPixelYUV(PixelYUV *result, const VisYUVFrame *frame, int x, int y) {
    if(x > frame->width || y > frame->height){
        return EFAULT;
    }
    PixelYUV *pix = &frame->data[x + frame->width * y];
    result->U = pix->U;
    result->Y = pix->Y;
    result->V = pix->V;
    return 0;
}

int YUVPixel_Draw(VisYUVFrame *frame, visBrush *brush, int x, int y) {
    if(x > frame->width || y > frame->height){
        return EFAULT;
    }
    PixelYUV *pix = &frame->data[x + frame->width * y];
    pix->U = brush->U;
    pix->Y = brush->Y;
    pix->V = brush->V;
    return 0;
}

