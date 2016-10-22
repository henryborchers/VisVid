/**
 * @file visDisplay.h
 */
#ifndef VISVID_VISVIEW_H
#define VISVID_VISVIEW_H


#include "visBuffer.h"
#include "visTypes.h"

/**
 * @struct visView
 * @brief Rendered data of the visualization.
 */
typedef struct {
    int width;              /**< Number of Pixels wide.*/
    int height;             /**< Number of Pixels high. */
    PixelValue *data;       /**< Raw data*/
}visView;

/**
 * Generates a visView type on the heap. Use DestroyVisView() to clean up afterwards.
 * @param width The number of pixels wide to create a new visView type.
 * @param height The number of pixels height to create a new visView type.
 * @return Returns a pointer to a new visView type.
 */
visView *CreateVisView(int width, int height);

void DestroyVisView(visView **pvd);

int visViewUpdate(visView *pD, visBuffer *buffer);

#endif //VISVID_VISVIEW_H