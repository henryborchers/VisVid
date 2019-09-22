//
// Created by Borchers, Henry Samuel on 10/22/16.
//
#include "visFrame.h"
#include "visTypes.h"
#include "visVisualization.h"
#include "visvid/visvid.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int visVisResult_CaculateBrightestOverWidth(visVisualResult *result, const VisYUVFrame *frame, PixelValue *sliceBuf){
    int frameHeight = -1;
    int frameWidth = -1;
    int rc = 0;


    rc = VisYUVFrame_GetSize(frame, &frameWidth, &frameHeight);
    if(rc != 0){
        return rc;
    }
    for(int x = 0; x < frameWidth; x++){
        PixelValue brightest = 0;
        for(int y = 0; y < frameHeight; y++){
            PixelValue resY;
            rc = VisYUVFrame_getPixelY(frame, x, y, &resY);
            if(resY > brightest){
                brightest = resY;
            }
            if(rc != 0){
                return rc;
            }

        }
        sliceBuf[x] = brightest;
    }
    rc = VisVisualResult_SetData(result, sliceBuf, (size_t) frameWidth);
    if(rc != 0){
        return rc;
    }
    return 0;
}

int visVisProcess(visVisualResult *pRes, const VisYUVFrame *pFrame, const visProcessContext *processContext, PixelValue *sliceBuffer) {
    int res;
    if(pRes == NULL || pFrame == NULL || processContext == NULL || processContext->processCb == NULL){
        return EFAULT;
    }

    if((res = processContext->processCb(pRes, pFrame, sliceBuffer)) != 0){
        return res;
    }
    return 0;
}
