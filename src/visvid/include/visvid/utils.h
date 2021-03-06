//
// Created by Borchers, Henry Samuel on 2018-11-29.
//
/**
 * @defgroup utilities Utility functions
 * @brief Avoid using function here
 */
#ifndef LIBVISVID_UTILS_H
#define LIBVISVID_UTILS_H
#include "visvid/visvid.h"
//#include <stdint.h>



typedef struct visBrush visBrush;
struct visBrush;
struct visBrush{
    PixelValue Y;     /**< @brief Luma. */
    PixelValue U;     /**< @brief Chroma. */
    PixelValue V;     /**< @brief Chroma. */
};

// DO NOT USE THESE FOLLOWING FUNCTIONS. They are for internal use and haven't been fully migrated out
/**
 * @internal Do not use
 */
int YUVPixel_Draw(VisYUVFrame *frame, const visBrush *brush, int x, int y);

/**
 * @internal Do not use
 */
int VisYUVFrame_getPixelYUV(PixelYUV *result, const VisYUVFrame *frame, int x, int y);
int VisYUVFrame_getPixelYUV2(const VisYUVFrame *frame, int x, int y, PixelValue* cy, PixelValue* cu, PixelValue* cv);
int VisYUVFrame_getPixelY(const VisYUVFrame *frame, int x, int y, PixelValue* cy);

/**
 * @internal Do not use
 */
int visImageRGB_readPixel(const visImageRGB *t, int pixelX, int pixelY, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);

#endif //LIBVISVID_UTILS_H
