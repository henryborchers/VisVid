//
// Created by Borchers, Henry Samuel on 10/22/16.
//
#include "visVisualization.h"
#include <errno.h>
#include <stdio.h>
#include "visFrame.h"
#include "visTypes.h"

int visVisResult_CaculateBrightestOverWidth(visVisualResult *result, VisYUVFrame *frame){
    int x, y;
    int frameHeight = -1;
    int frameWidth = -1;
    int rc = 0;



    rc = VisYUVFrame_GetSize(frame, &frameWidth, &frameHeight);
    if(rc != 0){
        return rc;
    }
    PixelValue slice[frameWidth];
    for(x = 0; x < frameWidth; x++){
        PixelValue brightest = 0;
        for(y = 0; y < frameHeight; y++){
            PixelYUV pix;
            rc = VisYUVFrame_getPixelYUV(&pix, frame, x, y);
            if(pix.Y > brightest){
                brightest = pix.Y;
            }
            if(rc != 0){
                return rc;
            }

        }
        slice[x] = brightest;
    }
    rc = VisVisualResult_SetData(result, slice, (size_t) frameWidth);
    if(rc != 0){
        return rc;
    };
    return 0;
}

int visVisProcess(visVisualResult *pRes, VisYUVFrame *pFrame, visProcessContext *processContext) {
    int res;
    if(pRes == NULL || pFrame == NULL || processContext == NULL || processContext->processCb == NULL){
        return EFAULT;
    }

    if((res = processContext->processCb(pRes, pFrame)) != 0){
        return res;
    }
    return 0;
}
