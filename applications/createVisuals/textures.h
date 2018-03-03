//
// Created by Borchers, Henry Samuel on 11/11/16.
//

#ifndef VISVID_TEXTURES_H
#define VISVID_TEXTURES_H

#include <stdint.h>

typedef struct {
    uint8_t *yPlane;
    uint8_t *uPlane;
    uint8_t *vPlane;
    int     yPitch;
    int     uPitch;
    int     vPitch;
    int     height;
    int     width;
}VisTexture;



int createRedBox(VisTexture *t, int height, int width);

int modify(VisTexture *t);

void destroyBox(VisTexture *t);

#endif //VISVID_TEXTURES_H
