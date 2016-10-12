/**
 * @file visDisplay.h
 */
#pragma once

#include "visBuffer.h"

typedef struct {
    int width;
    int height;
    uint8_t *data;
}visDisplay;

visDisplay *CreateVisDisplay(int width, int height);

void DestroyVisDisplay(visDisplay **pvd);

int visDisplayUpdate(visDisplay *pD, visBuffer *buffer);
