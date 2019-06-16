/**
 * @file visView.h
 */
#ifndef VISVID_VISVIEW_H
#define VISVID_VISVIEW_H

#include "visvid_export.h"
#include <stdint.h>
#include "visBuffer.h"
#include "visImageWriter.h"
#include "visTypes.h"
#include "visvid/visvid.h"

/**
 * Updates the view information based on the current data in the buffer
 * @param pView Pointer to a view to be updated.
 * @param buffer Pointer to a buffer which to build this information from.
 * @return Returns 0 on success.
 */
VISVID_DEPRECATED int visView_Update(visView *pView, visBuffer *buffer);
int visView_Update2(visView *pView, visBuffer *buffer);


/**
 * Callback for generating a black and white heatmap.
 */
int visViewRGBA_value2BW(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);


#endif //VISVID_VISVIEW_H