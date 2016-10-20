/**
 * @file visDisplay.h
 */
#ifndef VISVID_VISDISPLAY_H
#define VISVID_VISDISPLAY_H


#include "visBuffer.h"
#include "visTypes.h"

/**
 * @struct visDisplay
 * @brief Rendered data of the visualization.
 */
typedef struct {
    int width;              /**< Number of Pixels wide.*/
    int height;             /**< Number of Pixels high. */
    PixelValue *data;       /**< Raw data*/
}visDisplay;

/**
 * Generates a visDisplay type on the heap. Use DestroyVisDisplay() to clean up afterwards.
 * @param width The number of pixels wide to create a new visDisplay type.
 * @param height The number of pixels hight to create a new visDisplay type.
 * @return Returns a pointer to a new visDisplay type.
 */
visDisplay *CreateVisDisplay(int width, int height);

void DestroyVisDisplay(visDisplay **pvd);

int visDisplayUpdate(visDisplay *pD, visBuffer *buffer);

#endif //VISVID_VISDISPLAY_H