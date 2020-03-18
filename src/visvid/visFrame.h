/**
 * @file visFrame.h
 * @author Henry Borchers
 * @date October 8, 2016
 * @brief Contains all the information used to contains a frame and its pixel components.
 */

#ifndef VISVID_VISFRAME_H_H
#define VISVID_VISFRAME_H_H


#include "visvid/visvid.h"
#include "visvid/utils.h"


/**
 * @brief Creates a YUV pixel on the heap.
 * @return Pointer to the new pixel. Returns NULL on failure.
 */
PixelYUV *PixelYUV_Create();

/**
 * @brief Frees up memory on the heap used for the pixel.
 * @param pixel Pixel to be destroyed.
 */
void PixelYUV_Destroy(PixelYUV **pixel);

/**
 * Use this function to set the pixel values directly.
 * @brief Sets YUV value of a pixel.
 * @param pixel Pixel to be changed.
 * @param y New y/luma value to assign to the pixel.
 * @param u New u/chroma value to assign to the pixel.
 * @param v New v/chroma value to assign to the pixel.
 * @return Returns 0 on success.
 */
int PixelYUV_setValue(PixelYUV *pixel, PixelValue y, PixelValue u, PixelValue v);

/**
 * Use this function to get the pixel values.
 * @brief Get the component values for a given pixel.
 * @param pixel Pixel to read values.
 * @param y Pointer to store the y/luma value after reading the given pixels.
 * @param u Pointer to store the u/chroma value after reading the given pixels.
 * @param v Pointer to store the v/chroma value after reading the given pixels.
 * @return Returns 0 on success.
 */
int PixelYUV_getValue(const PixelYUV *pixel, PixelValue *y, PixelValue *u, PixelValue *v);

/**
 * Gets the time information for the frame.
 * @param frame The frame to read the time information from.
 * @param result Pointer to to store the time information of the frame.
 * @return Returns 0 on success.
 */
int VisYUVFrame_SetPos(VisYUVFrame *frame, int64_t *result);


int visYUVFrame_Fill(VisYUVFrame *frame, const visBrush *brush);


#endif //VISVID_VISFRAME_H_H
