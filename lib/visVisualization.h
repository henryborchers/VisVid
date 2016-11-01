//
// Created by Borchers, Henry Samuel on 10/22/16.
//
/**
 * @file visVisualization.h
 * @author Henry Borchers
 * @date October 22, 2016
 * @brief Contains all the function for calculating the visualizations.
 */
#ifndef VISVID_VISVISUALIZATION_H
#define VISVID_VISVISUALIZATION_H

#include "visResult.h"
#include "visFrame.h"
#include "visBuffer.h"
typedef struct visProcessContext visProcessContext;

struct visProcessContext{
    int(*processCb)(visVisualResult *result, VisYUVFrame *frame);
};


/**
 * The produces a similar result to a video waveform monitor.
 * @param result
 * @param frame
 * @return
 */
int visVisBrightest(visVisualResult *result, VisYUVFrame *frame);
int visVisProcess(visBuffer *pRes, VisYUVFrame *pFrame, visProcessContext *processContext);
#endif //VISVID_VISVISUALIZATION_H