//
// Created by Borchers, Henry Samuel on 2018-11-29.
//

#ifndef LIBVISVID_UTILS_H
#define LIBVISVID_UTILS_H
#include "visvid/visvid.h"
#include <stdint.h>



typedef struct visBrush visBrush;
struct visBrush;
struct visBrush{
    PixelValue Y;     /**< @brief Luma. */
    PixelValue U;     /**< @brief Chroma. */
    PixelValue V;     /**< @brief Chroma. */
};

int YUVPixel_Draw(VisYUVFrame *frame, struct visBrush *brush, int x, int y);

#endif //LIBVISVID_UTILS_H
