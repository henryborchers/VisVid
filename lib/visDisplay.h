/**
 * @file visDisplay.h
 */
#ifndef VISVID_VISDISPLAY_H
#define VISVID_VISDISPLAY_H


#include "visBuffer.h"
#include "visTypes.h"

typedef struct {
    int width;
    int height;
    PixelValue *data;
}visDisplay;

visDisplay *CreateVisDisplay(int width, int height);

void DestroyVisDisplay(visDisplay **pvd);

int visDisplayUpdate(visDisplay *pD, visBuffer *buffer);

#endif //VISVID_VISDISPLAY_H