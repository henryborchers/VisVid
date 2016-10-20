/**
 * @file visDisplay.h
 */
#pragma once

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
