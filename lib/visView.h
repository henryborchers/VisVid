/**
 * @file visView.h
 */
#ifndef VISVID_VISVIEW_H
#define VISVID_VISVIEW_H


#include "visBuffer.h"
#include "visTypes.h"
#include "visImageWriter.h"

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
visView *VisView_Create(int width, int height);

/**
 * Cleans up and free memory used in a visView type stored on the heap.
 * @param pvd Pointer to the visView to clean up and freed.
 */
void VisView_Destroy(visView **pvd);

/**
 * Updates the view information based on the current data in the buffer
 * @param pView Pointer to a view to be updated.
 * @param buffer Pointer to a buffer which to build this information from.
 * @return Returns 0 on success.
 */
int visView_Update(visView *pView, visBuffer *buffer);
int visView_Update2(visView *pView, visBuffer *buffer);
int visView_Update3(visView *pView, visBuffer *buffer);

/**
 * Creates a Black and white image in RGBA888 format to be used with other tools.
 * @param out A pointer to where to save the rendered image format.
 * @param pView A pointer to the view image.
 * @return Returns zero on success.
 */
//int visViewRGB_GenerateRGBA(visImageRGB *out, visView *pView);
int visViewRGB_GenerateRGBA(visImageRGB *out, visView *pView,
                             int(*callback)(PixelValue result, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a));
/**
 * Callback for generating a black and white heatmap.
 */
int visViewRGBA_value2BW(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);

/**
 * Callback for generating a color heatmap.
 */
int visViewRGBA_value2color1(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);
#endif //VISVID_VISVIEW_H