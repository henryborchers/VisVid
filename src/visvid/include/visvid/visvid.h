//
// Created by Borchers, Henry Samuel on 1/7/17.
//

#ifndef LIBVISVID_VISVID_H
#define LIBVISVID_VISVID_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t PixelValue;


/**
 * @struct PixelYUV
 * @brief The pixel split into it's YUV components.
 *
 * @note While YUV is really an analog color format and the true format is YCrBr,
 * it's much easier to write YUV as a variable name and YCrBr is more thought of in terms of their analog counterpart.
 */
typedef struct PixelYUV {
    PixelValue Y;    /**< @brief Luma value for a pixel.*/
    PixelValue U;    /**< @brief Cr chroma value for pixel.*/
    PixelValue V;    /**< @brief Br chroma value for pixel.*/
}PixelYUV;


typedef struct VisYUVFrame VisYUVFrame;

/**
 * @struct visVisualResult
 * @brief  Contains the calculated data from a visualization calculation.
 */
struct visVisualResult{
    bool ready;         /**< Declares if a result has been fully calculated.*/
    int size;           /**< The number of pixels wide the result is. */
    PixelValue *data ;   /**< Raw Data */
};
typedef struct visVisualResult visVisualResult;

struct visProcessContext{
    int(*processCb)(visVisualResult *result, VisYUVFrame *frame);
};
struct visProcessContext;
typedef struct visProcessContext visProcessContext;


/**
 * The produces a similar result to a video waveform monitor.
 * @param result
 * @param frame
 * @return
 */
int visVisResult_CaculateBrightestOverWidth(visVisualResult *result, VisYUVFrame *frame);
// TODO: Create a creater for visProcessContext.

int visVisProcess(visVisualResult *pRes, VisYUVFrame *pFrame, visProcessContext *processContext);

typedef struct visBufferNode visBufferNode;
/**
 * @struct visView
 * @brief Rendered data of the visualization.
 */
typedef struct visView{
    int width;              /**< Number of Pixels wide.*/
    int height;             /**< Number of Pixels high. */
    PixelValue *data;       /**< Raw data*/
}visView;

typedef struct visBuffer visBuffer;

/**
 * @struct visBuffer
 * @brief Used to store the sequence of calculation results from the visualization.
 */
struct visBuffer {
    size_t bufferMaxSize;               /**< Number of nodes in the buffer.*/
    size_t bufferLen;               /**< Number of nodes in the buffer.*/
    size_t bufferWidth;             /**< The resolution of the calculations*/
    visBufferNode *first;           /**< First node in the buffer*/
    visBufferNode *last;            /**< Last node in the buffer*/
};


/**
 *
 * @param width Number of pixels used by each result slice that will be stored in the buffer.
 * @param bufferSize The size of a buffer. If bufferSize is 0, the buffer size is unlimited.
 * @return Returns a pointer the new visBuffer.
 * @note You are responsible for freeing this data when you are done with it. Use DestroyVisBuffer() to release any
 * memory reserved by this.
 */
visBuffer *VisBuffer_Create2(size_t width, size_t bufferSize);


/**
 * @struct VisImageRGB
 * @brief RGB image data
 */
//struct visImageRGB{
typedef struct visImageRGB{
    uint8_t *plane;     /**< Raw data for pixel information.*/
    int     pitch;      /**< Pitch of the image.*/
    int     height;     /**< Height of the image.*/
    int     width;      /**< Width of the image.*/
} visImageRGB;

typedef struct visImageRGB visImageRGB;
/**
 * Allocates the amount of memory needed to store a the data for an RGBA8888 image. You are responsible for freeing
 * this data when you are finished with it by using freeImageRGB().
 * @param t Pointer to the VisImageRGB object to allocate the memory for.
 * @param height The number of pixels high for the desired image.
 * @param width The number of pixels wide for the desired image.
 * @return 0 Returns zero on success. Anything else is an error.
 */
int visImageRGB_Alloc(visImageRGB *t, int width, int height);

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
 * Pushes a visVisualResult to the end of the buffer.
 * @param buffer The Buffer to push a visVisualResult to the end of.
 * @param pRes The visVisualResult to push to the end of a visBuffer.
 * @return Returns 0 on success;
 */
int visBuffer_PushBackResult(visBuffer *buffer, visVisualResult *pRes);


/**
 * Sets the pixel dimensions of a given frame.
 * @param frame Pointer to a frame in which to size or resize.
 * @param width Change the width of the frame to given value.
 * @param height Change the height of the frame to given value.
 * @return
 */
int VisYUVFrame_SetSize(VisYUVFrame *frame, int width, int height);


/**
 * Sizes or re-sizes the length of the visVisualResult. Any existing data will be lost.
 * @param pRest Pointer to visVisualResult to set the length.
 * @param size The new length to set the result to. This is most like the width or the height of the frame.
 * @return Returns 0 on success.
 */
int VisVisualResult_SetSize(visVisualResult *pRest, size_t size);

/**
 * Generates a visView type on the heap. Use DestroyVisView() to clean up afterwards.
 * @param width The number of pixels wide to create a new visView type.
 * @param height The number of pixels height to create a new visView type.
 * @return Returns a pointer to a new visView type.
 */
visView *VisView_Create(int width, int height);



/**
 * Performs a memory copy of the data and sets visVisualResult->ready to true.
 * @param pRes visVisualResult with data to copy to.
 * @param data The data to copy from.
 * @param length The number of elements in the data.
 * @return Returns 0 on success.
 */
int VisVisualResult_SetData(visVisualResult *pRes, PixelValue *data, size_t length);
int VisVisualResult_Init(visVisualResult *newResult);
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
 * Callback for generating a color heatmap.
 */
int visViewRGBA_value2color1(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a);


/**
 * Frees up the memory used by a given visVisualResult.
 * @param pRes Pointer to the visVisualResult to free up.
 */
void VisVisualResult_Destroy(visVisualResult **pRes);

void VisVisualResult_Cleanup(visVisualResult *pRes);


/**
 * Frees up the data being used by a VisImageRGB object
 * @param t VisImageRGB to write pixel data to
 */
void visImageRGB_FreeData(visImageRGB *t);


/**
 * Frees up the memory used by the given frame.
 * @param frame The frame to cleaned up.
 */
void VisYUVFrame_Destroy(VisYUVFrame **frame);


/**
 * Cleans up and destroys the given visBuffer.
 * @param buffer The visBuffer to be cleaned up.
 */
void VisBuffer_Destroy(visBuffer **buffer);

/**
 * Cleans up and free memory used in a visView type stored on the heap.
 * @param pvd Pointer to the visView to clean up and freed.
 */
void VisView_Destroy(visView **pvd);

/**
 * Gets the size of length of a visVisualResult.
 * @param size A pointer to where the length can be stored.
 * @param pRest Pointer to visVisualResult to get the length.
 * @return Returns 0 on success.
 */
int VisVisualResult_GetSize(int *size, visVisualResult *pRest);


/**
 * Get the pixel dimensions of a given frame.
 * @param frame The frame to read the dimensions from.
 * @param width Pointer to an int to store the width information of the frame.
 * @param height Pointer to an int to store the width information of the frame.
 * @return Returns 0 on success.
 */
int VisYUVFrame_GetSize(VisYUVFrame *frame, int *width, int *height);


#endif //LIBVISVID_VISVID_H


