/**
 * @file visResult.h
 * @author Henry Borchers
 * @date 10/11/2016
 */
#ifndef VISVID_VISRESULT_H
#define VISVID_VISRESULT_H

#include <stdbool.h>
#include <stddef.h>
#include "visTypes.h"



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

/**
 * Creates a new visVisualResult on the heap and returns a pointer to it.
 * @return Pointer to new visVisualResult.
 */
visVisualResult *VisVisualResult_Create();

/**
 * Frees up the memory used by a given visVisualResult.
 * @param pRes Pointer to the visVisualResult to free up.
 */
void VisVisualResult_Destroy(visVisualResult **pRes);

/**
 * Checks the result has valid data from a calculation
 */
bool VisVisualResult_IsReady(visVisualResult *pRes);

/**
 * Sizes or re-sizes the length of the visVisualResult. Any existing data will be lost.
 * @param pRest Pointer to visVisualResult to set the length.
 * @param size The new length to set the result to. This is most like the width or the height of the frame.
 * @return Returns 0 on success.
 */
int VisVisualResult_SetSize(visVisualResult *pRest, int size);

/**
 * Gets the size of length of a visVisualResult.
 * @param size A pointer to where the length can be stored.
 * @param pRest Pointer to visVisualResult to get the length.
 * @return Returns 0 on success.
 */
int VisVisualResult_GetSize(int *size, visVisualResult *pRest);

/**
 * Get the value of a visVisualResult at a certain offset.
 * @param value A pointer where the results can be saved.
 * @param pRes The visVisualResult to retrieve the value from.
 * @param offset The index for the value in the data.
 * @return Returns 0 on success.
 */
int VisVisualResult_GetValue(PixelValue *value, visVisualResult *pRes, int offset);

/**
 * Performs a memory copy of the data and sets visVisualResult->ready to true.
 * @param pRes visVisualResult with data to copy to.
 * @param data The data to copy from.
 * @param length The number of elements in the data.
 * @return Returns 0 on success.
 */
int VisVisualResult_SetData(visVisualResult *pRes, PixelValue *data, size_t length);

int VisVisualResult_Init(visVisualResult *newResult);

int VisVisualResult_copy(visVisualResult *dst, visVisualResult *src);
#endif //VISVID_VISRESULT_H