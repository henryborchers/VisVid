//
// Created by Borchers, Henry Samuel on 10/22/16.
//
#include <errno.h>
#include "visFrame.h"
#include "visVisualization.h"
int visVisBrightest(visVisualResult *result, VisYUVFrame *frame){
    int x, y;
    int frameHeight = -1;
    int frameWidth = -1;
    int rc = 0;



    rc = GetVisYUVFrameSize(frame,&frameWidth, &frameHeight);
    if(rc != 0){
        return rc;
    }
    PixelValue slice[frameWidth];
    for(x = 0; x < frameWidth; x++){
        PixelValue brightest = 0;
        for(y = 0; y < frameHeight; y++){
            PixelYUV pix;
            rc = GetPixelFromYUVFrame(&pix, frame, x, y);
            if(pix.Y > brightest){
                brightest = pix.Y;
            }
            if(rc != 0){
                return rc;
            }

        }
        slice[x] = brightest;
    }
    rc = SetVisVisualResultData(result, slice, (size_t)frameWidth);
    if(rc != 0){
        return rc;
    };

    return 0;
}

int visVisProcess(visBuffer *pRes, VisYUVFrame *pFrame, visProcessContext *processContext) {
    if(pRes == NULL || pFrame == NULL || processContext == NULL || processContext->processCb == NULL){
        return EFAULT;
    }
    return 0;
}
