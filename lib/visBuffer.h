//
// Created by Borchers, Henry Samuel on 10/8/16.
//
/**
 * @file visBuffer.h
 * @author Henry Borchers
 * @date October 8, 2016
 * @brief Contains all logic for the buffer and the visualization caluculation results
 */

#ifndef VISVID_VISBUFFER_H
#define VISVID_VISBUFFER_H

typedef struct _visBufferNode visBufferNode;
typedef struct _visBuffer visBuffer;
typedef struct _visVisualResult visVisualResult;

/**
 * Creates a new visVisualResult on the heap and returns a pointer to it.
 * @return Pointer to new visVisualResult.
 */
visVisualResult *CreateVisVisualResult();

/**
 * Frees up the memory used by a given visVisualResult.
 * @param pRes Pointer to the visVisualResult to free up.
 */
void DestroyVisVisualResult(visVisualResult **pRes);

/**
 * Checks the result has valid data from a calculation
 */
bool isVisVisualResultReady(visVisualResult *pRes);

/**
 * Sizes or resizes the length of the visVisualResult. Any existing data will be lost.
 * @param pRest Pointer to visVisualResult to set the length.
 * @param size The new length to set the result to. This is most like the width or the height of the frame.
 * @return Returns 0 on success.
 */
int SetVisVisualResultReadySize(visVisualResult *pRest, int size);

/**
 * Gets the size of length of a visVisualResult.
 * @param pRest Pointer to visVisualResult to get the length.
 * @param size A pointer to where the length can be stored.
 * @return Returns 0 on success.
 */
int GetVisVisualResultReadySize(visVisualResult *pRest, int *size);

/**
 * Get the value of a visVisualResult at a certain offset.
 * @param pRes The visVisualResult to retrieve the value from.
 * @param value A pointer where the results can be saved.
 * @param offset The index for the value in the data.
 * @return Returns 0 on success.
 */
int GetVisVisualResultValue(visVisualResult *pRes, uint8_t *value, uint16_t offset);

/**
 * Performs a memory copy of the data and sets visVisualResult->ready to true.
 * @param pRes visVisualResult with data to copy to.
 * @param data The data to copy from.
 * @param length The number of elements in the data.
 * @return Returns 0 on success.
 */
int SetVisVisualResultData(visVisualResult *pRes, uint8_t *data, size_t length);

/**
 * Creates a new visBuffer on the heap.
 * @return Returns a pointer the new visBuffer.
 */
visBuffer *CreateVisBuffer();

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
 * Gets the first node of the visBuffer.
 * @param buffer The buffer to find the first node.
 * @return Returns a node in the visBuffer.
 */
static visBufferNode *visBufferFront(visBuffer *buffer);

/**
 * Gets the size of the buffer stored in the buffer's metadata.
 * @param buffer The visBuffer to see the size.
 * @return The number of nodes inside the buffer.
 */
size_t visBufferSize(visBuffer *buffer);

/**
 * Gets the next node after the given one.
 * @param node The node which to find the next node.
 * @return Returns a pointer to the next node. Returns NULL if none exists.
 */
static visBufferNode *visBufferNextNode(visBufferNode *node);

/**
 * Gets the nrevious node after the given one.
 * @param node The node which to find the previous node.
 * @return Returns a pointer to the previous node. Returns NULL if none exists.
 */
static visBufferNode *visBufferPreviousNode(visBufferNode *node);

/**
 * Pushes a visBufferNode to the end of a visBuffer.
 * @param buffer The buffer to add the node to.
 * @param newNode The node to add to the buffer.
 * @return Returns 0 on success.
 */
static int visBufferPushBack(visBuffer *buffer, visBufferNode *newNode);


/**
 * Pushes a visVisualResult to the end of the buffer.
 * @param buffer The Buffer to push a visVisualResult to the end of.
 * @param pRes The visVisualResult to push to the end of a visBuffer.
 * @return Returns 0 on success;
 */
int visBufferPushBackResult(visBuffer *buffer, visVisualResult *pRes);

/**
 * Pops a visBufferNode off the front of the a visBuffer.
 * @param buffer The visBuffer to pop a visBufferNode off of.
 * @return Returns a pointer to the visBufferNode that was popped off of the front of the visBuffer.
 */
static visBufferNode *visBufferPop(visBuffer *buffer);

/**
 * Pops a visVisualResult off the front of the a visBuffer.
 * @param buffer The visBuffer to pop a visVisualResult off of.
 * @return Returns a pointer to the visVisualResult that was popped off of the front of the visBuffer.
 */
visVisualResult *visBufferPopResult(visBuffer *buffer);

/**
 * Creates a new visBufferNode from a visVisualResult.
 * @param pRes The Result to added to node.
 * @return Returns a new visBufferNode with the visVisualResult.
 */
static visBufferNode *CreateVisBufferNode(visVisualResult *pRes);


/**
 * Cleans up the data in a bufferNode.
 * @param node The Node to be cleaned up.
 */
void DestroyVisBufferNode(visBufferNode **node);

/**
 * Get the visVisualResult from a given visBufferNode.
 * @param pNode The visBufferNode to get the visVisualResult from.
 * @return Returns a pointer to visVisualResult.
 */
static visVisualResult *visBufferNodeResult(visBufferNode *pNode);

#endif //VISVID_VISBUFFER_H
