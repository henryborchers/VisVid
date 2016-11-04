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

typedef struct visBufferNode visBufferNode;
typedef struct visBuffer visBuffer;

/**
 * Creates a new visBuffer on the heap.
 * @return Returns a pointer the new visBuffer.
 */
visBuffer *CreateVisBuffer(size_t width);

/**
 * Cleans up and destroys the given visBuffer.
 * @param buffer The visBuffer to be cleaned up.
 */
void DestroyVisBuffer(visBuffer **buffer);


/**
 * Check to see if the buffer is empty.
 * @param buffer The buffer to check if it's empty.
 * @return Returns 0 if the buffer is not empty and returns 1 the buffer is empty.
 */
int visBufferIsEmpty(visBuffer *buffer);

/**
 * Gets the size of the buffer stored in the buffer's metadata.
 * @param buffer The visBuffer to see the size.
 * @return The number of nodes inside the buffer.
 */
size_t visBufferLength(visBuffer *buffer);



/**
 * Pushes a visVisualResult to the end of the buffer.
 * @param buffer The Buffer to push a visVisualResult to the end of.
 * @param pRes The visVisualResult to push to the end of a visBuffer.
 * @return Returns 0 on success;
 */
int visBufferPushBackResult(visBuffer *buffer, visVisualResult *pRes);

/**
 * Pops a visVisualResult off the front of the a visBuffer.
 * @param buffer The visBuffer to pop a visVisualResult off of.
 * @return Returns a pointer to the visVisualResult that was popped off of the front of the visBuffer.
 */
visVisualResult *visBufferPopResult(visBuffer *buffer);


/**
 * Cleans up the data in a bufferNode.
 * @param node The Node to be cleaned up.
 */
void DestroyVisBufferNode(visBufferNode **node);

visBufferNode * _getBufferNode(visBuffer *buffer, size_t index);

size_t _nodePosition(visBufferNode *node);
/**
 *
 * @param pRes
 * @param buffer
 * @param index
 * @return Returns 0 when the result is null. Returns a 1 when a valid result is found. Returns a negative number on failure.
 */
int getResult(PixelValue *pRes, visBuffer *buffer, size_t index);
#endif //VISVID_VISBUFFER_H
