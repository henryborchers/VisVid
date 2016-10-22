/**
 * @file visView.h
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

/**
 * Cleans up and free memory used in a visView type stored on the heap.
 * @param pvd Pointer to the visView to clean up and freed.
 */
void DestroyVisView(visView **pvd);

/**
 * Updates the view information based on the current data in the buffer
 * @param pView Pointer to a view to be updated.
 * @param buffer Pointer to a buffer which to build this information from.
 * @return Returns 0 on success.
 */
int visViewUpdate(visView *pView, visBuffer *buffer);

#endif //VISVID_VISVIEW_H