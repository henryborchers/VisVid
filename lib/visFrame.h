/**
 * @file visFrame.h
 * @brief Contains all the infomation used to contains a frame and its pixel components.
 */
//
// Created by Borchers, Henry Samuel on 10/8/16.
//



#ifndef VISVID_VISFRAME_H_H
#define VISVID_VISFRAME_H_H

typedef struct _PixelYUV PixelYUV;
typedef struct _VisYUVFrame VisYUVFrame;

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
int SetPixelYUV(PixelYUV *pixel, uint8_t y, uint8_t u, uint8_t v);

/**
 * Use this function to get the pixel values.
 * @param pixel Pixel to read values.
 * @param y Pointer to store the y/luma value after reading the given pixels.
 * @param u Pointer to store the u/chroma value after reading the given pixels.
 * @param v Pointer to store the v/chroma value after reading the given pixels.
 * @return Returns 0 on success.
 */
int GetPixelYUV(PixelYUV *pixel, uint8_t *y, uint8_t *u, uint8_t *v);


#endif //VISVID_VISFRAME_H_H
