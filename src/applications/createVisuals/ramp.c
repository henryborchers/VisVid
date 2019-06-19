//
// Created by Borchers, Henry Samuel on 11/22/16.
//

#include "ramp.h"
#include <stdint.h>
#include <stdio.h>

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
}

int vertical_ramp(visImageRGB *t) {
    int height = t->height;
    int width = t->width;
    for(int y = 0; y < height; y++){

//    uint8_t value = y;
        uint8_t value = (uint8_t)(((float) y / height) * 255);
        printf("y= %d, value = %d\n", y, value);
        for(int x = 0; x < width; x++){
//            uint8_t value = (uint8_t)(((float) x / t->width) * 255);

//            visImageRGB_WritePixel(t, x, 100, value, value, value, value);
            visImageRGB_WritePixel(t, x, y, value, value, value, value);
//             write_pixel(t, y, x, 1, 100, 2, (uint32_t)(((float)x /t->width - count) * 255));
        }
    }

    return 0;
}
