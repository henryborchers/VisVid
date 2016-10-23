/**
 * @file visFrame.h
 * @author Henry Borchers
 * @date October 8, 2016
 * @brief Contains all the information used to contains a frame and its pixel components.
 */

#ifndef VISVID_VISFRAME_H_H
#define VISVID_VISFRAME_H_H

#include "visTypes.h"
//const char VISFRAME_VERSION[] = "0.0.1";

typedef struct PixelYUV PixelYUV;
typedef struct VisYUVFrame VisYUVFrame;
typedef struct visBrush visBrush;
struct visBrush{
    PixelValue Y;
    PixelValue U;
    PixelValue V;
};

/**
 * @struct PixelYUV
 * @brief The pixel split into it's YUV components
 *
 * This represent a pixel's value. NOTE: While YUV is really an analog color format and the true format is YCrBr,
 * it's much easier to write YUV as a variable name and YCrBr is more thought of in terms of their analog counterpart.
 */
struct PixelYUV {
    PixelValue Y;    /**< Luma value for a pixel.*/
    PixelValue U;    /**< Cr chroma value for pixel.*/
    PixelValue V;    /**< Br chroma value for pixel.*/
};



/**
 * Creates a YUV pixel on the heap.
 * @return Pointer to the new pixel. Returns NULL on failure.
 */
PixelYUV *CreatePixelYUV();

/**
 * Frees up memory on the heap used for the pixel.
 * @param pixel Pixel to be destroyed.
 */
void DestroyPixelYUV(PixelYUV **pixel);

/**
 * Use this function to set the pixel values directly.
 * @param pixel Pixel to be changed.
 * @param y New y/luma value to assign to the pixel.
 * @param u New u/chroma value to assign to the pixel.
 * @param v New v/chroma value to assign to the pixel.
 * @return Returns 0 on success.
 */
int SetPixelValueYUV(PixelYUV *pixel, PixelValue y, PixelValue u, PixelValue v);

/**
 * Use this function to get the pixel values.
 * @param pixel Pixel to read values.
 * @param y Pointer to store the y/luma value after reading the given pixels.
 * @param u Pointer to store the u/chroma value after reading the given pixels.
 * @param v Pointer to store the v/chroma value after reading the given pixels.
 * @return Returns 0 on success.
 */
int GetPixelValueYUV(PixelYUV *pixel, PixelValue *y, PixelValue *u, PixelValue *v);

/**
 * Allocates a new frame on the heap with zero pixels high and and zero pixels wide and returns a pointer to it's
 * location.
 * @return Pointer to new frame.
 */
VisYUVFrame *CreateVisYUVFrame();

/**
 * Frees up the memory used by the given frame.
 * @param frame The frame to cleaned up.
 */
void DestroyVisYUVFrame(VisYUVFrame **frame);

/**
 * Sets the pixel dimensions of a given frame.
 * @param frame Pointer to a frame in which to size or resize.
 * @param width Change the width of the frame to given value.
 * @param height Change the height of the frame to given value.
 * @return
 */
int SetVisYUVFrameSize(VisYUVFrame *frame, int width, int height);

/**
 * Get the pixel dimensions of a given frame.
 * @param frame The frame to read the dimensions from.
 * @param width Pointer to an int to store the width information of the frame.
 * @param height Pointer to an int to store the width information of the frame.
 * @return Returns 0 on success.
 */
int GetVisYUVFrameSize(VisYUVFrame *frame, int *width, int *height);

/**
 * Gets the time information for the frame.
 * @param frame The frame to read the time information from.
 * @param pos Pointer to to store the time information of the frame.
 * @return Returns 0 on success.
 */
int GetVisYUVFrameSizePos(VisYUVFrame *frame, int64_t *pos);

int GetPixelFromYUVFrame(PixelYUV *pixel, VisYUVFrame *frame, int x, int y);

int visFillYUVFrame(VisYUVFrame *frame, visBrush *brush);

int visDrawYUVPixel(VisYUVFrame *frame, visBrush *brush, int x, int y);

#endif //VISVID_VISFRAME_H_H
