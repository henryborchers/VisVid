//
// Created by Borchers, Henry Samuel on 11/11/16.
//

#include <stdlib.h>
#include <string.h>
#include "textures.h"

int createRedBox(VisTexture *t, int height, int width) {
    t->yPitch = width;
    t->yPlane = malloc(sizeof(uint8_t) * t->yPitch * height);
    memset(t->yPlane,76, t->yPitch * height);

    t->uPitch = width/2;
    t->uPlane = malloc(sizeof(uint8_t) * t->uPitch * height);
    memset(t->uPlane,84, t->uPitch * height);

    t->vPitch = width/2;
    t->vPlane = malloc(sizeof(uint8_t) * t->vPitch * height);
    memset(t->vPlane, 84, t->vPitch * height);

    t->height = height;
    t->width = width;
    return 0;
}

void destroyBox(VisTexture *t) {

    if(t->yPlane != NULL){
        free(t->yPlane);
        t->yPlane = NULL;
    }

    if(t->uPlane != NULL){
        free(t->uPlane);
        t->uPlane = NULL;
    }

    if(t->vPlane != NULL){
        free(t->vPlane);
        t->vPlane = NULL;
    }


}

int modify(VisTexture *t) {
    static uint8_t count = 0;

    for(int y = 0; y < t->height; y++){
        for(int x = 0; x < t->yPitch; x++){
            t->yPlane[(x + t->yPitch * y)] = (((float)x/t->yPitch) * 256) - count;
        }
        for(int x = 0; x < t->uPitch; x++){
            t->uPlane[(x + t->uPitch * y)] = (((float)x/t->uPitch) * 256) + count;
        }
        for(int x = 0; x < t->vPitch; x++){
            t->uPlane[(x + t->vPitch * y)] = (((float)x/t->vPitch) * 256) + count;
        }
    }

    count++;
//    memset(t->uPlane, last_U + 1, t->uPitch * t->height/2);

    return 0;
}
