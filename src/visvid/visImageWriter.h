/**
 * @file visImageWriter.h
 * @author Henry Borchers
 * @date November 11, 2016
 * @brief Contains everything for writing image data.
 */


#ifndef VISVID_VISIMAGES_H
#define VISVID_VISIMAGES_H

#include "visvid/visvid.h"
#include <stdint.h>

/**
 *
 * Writes a pixel value to a allocated VisImageRGB object.
 * @param t VisImageRGB to write pixel data to
 * @param pixelY The y coordinate to write to.
 * @param pixelX The x coordinate to write to.
 * @param r The amount of red to be used in the given pixel.
 * @param g The amount of green to be used in the given pixel.
 * @param b The amount of blue to be used in the given pixel.
 * @param a The amount of assigned to the alpha channel to be used in the given pixel.
 * @return Returns zero on success.
 */
int visImageRGB_WritePixel(visImageRGB *t, int pixelX, int pixelY, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

int visImageRGB_readPixel(visImageRGB *t, int pixelX, int pixelY, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);

#endif //VISVID_VISIMAGES_H
