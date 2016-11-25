//
// Created by Borchers, Henry Samuel on 11/22/16.
//

#include "generators.h"

int ramp(visImageRGB *t) {
    static uint8_t count = 0;

    for(int y = 0; y < t->height; y++){
        for(int x = 0; x < t->width; x++){
            visImageRGB_WritePixel(t, x, y, 0, count, (uint8_t) (((float) x / t->width - count) * 255), 255);
//             write_pixel(t, y, x, 1, 100, 2, (uint32_t)(((float)x /t->width - count) * 255));
        }
    }

    count++;

    return 0;
    return 0;
}
