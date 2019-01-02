/**
 * @file visFrame.h
 * @author Henry Borchers
 * @date October 8, 2016
 * @brief Contains all the information used to contains a frame and its pixel components.
 */

#ifndef VISVID_VISFRAME_H_H
#define VISVID_VISFRAME_H_H

#ifdef MEMORY_DEBUG
#include <stdlib.h>
extern void *d_debug_mem_malloc(size_t size, char *file, size_t line);
extern void *d_debug_mem_calloc(size_t size, size_t num, char *file, size_t line);
#define malloc(size) d_debug_mem_malloc(size, __FILE__, __LINE__)
#define calloc(size, num) d_debug_mem_calloc(size, num, __FILE__, __LINE__)
#endif

//#include <stdint.h>
#include "visvid/visvid.h"
#include "visvid/utils.h"
//#include "visTypes.h"
//struct PixelYUV;




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
int PixelYUV_getValue(PixelYUV *pixel, PixelValue *y, PixelValue *u, PixelValue *v);

/**
 * Gets the time information for the frame.
 * @param frame The frame to read the time information from.
 * @param result Pointer to to store the time information of the frame.
 * @return Returns 0 on success.
 */
int VisYUVFrame_SetPos(VisYUVFrame *frame, int64_t *result);


int visYUVFrame_Fill(VisYUVFrame *frame, struct visBrush *brush);


#endif //VISVID_VISFRAME_H_H
