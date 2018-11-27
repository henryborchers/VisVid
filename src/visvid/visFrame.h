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

#include <stdint.h>
#include "visvid.h"
//#include "visTypes.h"
struct PixelYUV;
struct visBrush;

typedef struct PixelYUV PixelYUV;

typedef struct visBrush visBrush;
struct visBrush{
    PixelValue Y;     /**< @brief Luma. */
    PixelValue U;     /**< @brief Chroma. */
    PixelValue V;     /**< @brief Chroma. */
};

/**
 * @struct PixelYUV
 * @brief The pixel split into it's YUV components.
 *
 * @note While YUV is really an analog color format and the true format is YCrBr,
 * it's much easier to write YUV as a variable name and YCrBr is more thought of in terms of their analog counterpart.
 */
struct PixelYUV {
    PixelValue Y;    /**< @brief Luma value for a pixel.*/
    PixelValue U;    /**< @brief Cr chroma value for pixel.*/
    PixelValue V;    /**< @brief Br chroma value for pixel.*/
};



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
 * Allocates a new frame on the heap with zero pixels high and and zero pixels wide and returns a pointer to it's
 * location.
 * @brief Create a new VisYUVFrame.
 * @return Pointer to new frame.
 * @note You are responsible for freeing up any memory when done. Use DestroyVisYUVFrame() to release any memory
 * reserved on the heap.
 */
VisYUVFrame *VisYUVFrame_Create();

/**
 * Frees up the memory used by the given frame.
 * @param frame The frame to cleaned up.
 */
void VisYUVFrame_Destroy(VisYUVFrame **frame);

/**
 * Sets the pixel dimensions of a given frame.
 * @param frame Pointer to a frame in which to size or resize.
 * @param width Change the width of the frame to given value.
 * @param height Change the height of the frame to given value.
 * @return
 */
int VisYUVFrame_SetSize(VisYUVFrame *frame, int width, int height);

/**
 * Get the pixel dimensions of a given frame.
 * @param frame The frame to read the dimensions from.
 * @param width Pointer to an int to store the width information of the frame.
 * @param height Pointer to an int to store the width information of the frame.
 * @return Returns 0 on success.
 */
int VisYUVFrame_GetSize(VisYUVFrame *frame, int *width, int *height);

/**
 * Gets the time information for the frame.
 * @param frame The frame to read the time information from.
 * @param result Pointer to to store the time information of the frame.
 * @return Returns 0 on success.
 */
int VisYUVFrame_SetPos(VisYUVFrame *frame, int64_t *result);

int VisYUVFrame_getPixelYUV(PixelYUV *result, VisYUVFrame *frame, int x, int y);

int visYUVFrame_Fill(VisYUVFrame *frame, visBrush *brush);

int YUVPixel_Draw(VisYUVFrame *frame, visBrush *brush, int x, int y);

#endif //VISVID_VISFRAME_H_H
