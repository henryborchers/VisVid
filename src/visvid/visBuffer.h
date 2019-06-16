//
// Created by Borchers, Henry Samuel on 10/8/16.
//
/**
 * @file visBuffer.h
 * @author Henry Borchers
 * @date October 8, 2016
 * @brief Contains all logic for the buffer and the visualization calculation results.
 */

#ifndef VISVID_VISBUFFER_H
#define VISVID_VISBUFFER_H

#include "visResult.h"
#include "visvid/visvid.h"
//#include "visTypes.h"
#include "visUtils.h"
#include "visvid_export.h"


typedef struct visBufferNode visBufferNode;


/**
 * @struct visBufferNode
 * @brief Wraps the visVisualResult into a linked list.
 */
struct visBufferNode {
    visVisualResult *result;
    visBufferNode *previous;
    visBufferNode *next;
    size_t position;
};

typedef struct visBuffer visBuffer;

/**
 * @fn visBuffer *VisBuffer_Create(size_t width);
 * @deprecated Use VisBuffer_Create2() instead.
 * @brief Creates a new visBuffer on the heap.
 * @param width Number of pixels used by each result slice that will be stored in the buffer.
 * @return Returns a pointer the new visBuffer.
 * @note You are responsible for freeing this data when you are done with it. Use DestroyVisBuffer() to release any
 * memory reserved by this.
 */
VISVID_DEPRECATED visBuffer *VisBuffer_Create(size_t width);


/**
 * Check to see if the buffer is empty.
 * @param buffer The buffer to check if it's empty.
 * @return Returns 0 if the buffer is not empty and returns 1 the buffer is empty.
 */
int visBuffer_isEmpty(const visBuffer *buffer);

/**
 * Gets the size of the buffer stored in the buffer's metadata.
 * @param buffer The visBuffer to see the size.
 * @return The number of nodes inside the buffer.
 */
size_t visBuffer_getLength(const visBuffer *buffer);

/**
 * Pops a visVisualResult off the front of the a visBuffer.
 * @param buffer The visBuffer to pop a visVisualResult off of.
 * @return Returns a pointer to the visVisualResult that was popped off of the front of the visBuffer.
 */
visVisualResult *visBuffer_PopResult(visBuffer *buffer);

visVisualResult *visBuffer_PopShiftResult(visBuffer *buffer);

/**
 * Cleans up the data in a bufferNode.
 * @param node The Node to be cleaned up.
 */
void VisBufferNode_Destroy(visBufferNode **node);

visBufferNode * _BufferNode_get(const visBuffer *buffer, size_t index);

int _nodePosition(visBufferNode *node);
/**
 *
 * @param pRes
 * @param buffer
 * @param index
 * @return Returns 0 when the result is null. Returns a 1 when a valid result is found. Returns a negative number on failure.
 */
int visBuffer_getResult(PixelValue *pRes, const visBuffer *buffer, size_t index);

int visBuffer_ShiftLeft(visBuffer *pBuffer);
#endif //VISVID_VISBUFFER_H
