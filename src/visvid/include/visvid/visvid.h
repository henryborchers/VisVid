//
// Created by Borchers, Henry Samuel on 1/7/17.
//

#ifndef LIBVISVID_VISVID_H
#define LIBVISVID_VISVID_H
#include <stdint.h>
#include <stdbool.h>
//#include "visTypes.h"
//#include "visFrame.h"
//#include "visBuffer.h"
//#include "visImageWriter.h"
//#include "visResult.h"
//#include "visView.h"
//#include "visFrame.h"
//#include "visVisualization.h"
//#include "visVersion.h"

typedef uint8_t PixelValue;

typedef struct VisYUVFrame VisYUVFrame;
/**
 * @struct visVisualResult
 * @brief  Contains the calculated data from a visualization calculation.
 */

struct visVisualResult{
    bool ready;         /**< Declares if a result has been fully calculated.*/
    int size;           /**< The number of pixels wide the result is. */
    PixelValue *data ;   /**< Raw Data */
};
typedef struct visVisualResult visVisualResult;

struct visProcessContext{
    int(*processCb)(visVisualResult *result, VisYUVFrame *frame);
};
struct visProcessContext;
typedef struct visProcessContext visProcessContext;


/**
 * The produces a similar result to a video waveform monitor.
 * @param result
 * @param frame
 * @return
 */
int visVisResult_CaculateBrightestOverWidth(visVisualResult *result, VisYUVFrame *frame);
// TODO: Create a creater for visProcessContext.

int visVisProcess(visVisualResult *pRes, VisYUVFrame *pFrame, visProcessContext *processContext);

#endif //LIBVISVID_VISVID_H


