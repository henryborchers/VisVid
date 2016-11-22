/**
 * @file visImageWriter.h
 * @author Henry Borchers
 * @date November 11, 2016
 * @brief Contains everything for writing image data.
 */


#ifndef VISVID_VISIMAGES_H
#define VISVID_VISIMAGES_H

#include <stdint.h>

typedef struct visImageRGB visImageRGB;

/**
 * @struct VisImageRGB
 * @brief RGB image data
 */
struct visImageRGB{
//typedef struct {
    uint8_t *plane;     /**< Raw data for pixel information.*/
    int     pitch;      /**< Pitch of the image.*/
    int     height;     /**< Height of the image.*/
    int     width;      /**< Width of the image.*/
};

/**
 * Allocates the amount of memory needed to store a the data for an RGBA8888 image. You are responsible for freeing
 * this data when you are finished with it by using freeImageRGB().
 * @param t Pointer to the VisImageRGB object to allocate the memory for.
 * @param height The number of pixels high for the desired image.
 * @param width The number of pixels wide for the desired image.
 * @return 0 Returns zero on success. Anything else is an error.
 */
int visAllocImageRGB(visImageRGB *t, int width, int height);

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
int visImageWritePixelRGB(visImageRGB *t, int pixelX, int pixelY, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/**
 * Frees up the data being used by a VisImageRGB object
 * @param t VisImageRGB to write pixel data to
 */
void visFreeImageRGB(visImageRGB *t);

#endif //VISVID_VISIMAGES_H