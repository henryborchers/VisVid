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

visBuffer *CreateVisBuffer();

void DestroyVisBuffer(visBuffer **buffer);

int visBufferIsEmpty(visBuffer *buffer);

static visBufferNode *visBufferFront(visBuffer *buffer);

size_t visBufferSize(visBuffer *buffer);

static visBufferNode *visBufferNextNode(visBufferNode *node);

static visBufferNode *visBufferPreviousNode(visBufferNode *node);

static int visBufferPushBack(visBuffer *buffer, visBufferNode *newNode);

int visBufferPushBackResult(visBuffer *buffer, visVisualResult *pRes);

static visBufferNode *visBufferPop(visBuffer *buffer);

visVisualResult *visBufferPopResult(visBuffer *buffer);

static visBufferNode *CreateVisBufferNode(visVisualResult *pRes);

void DestroyVisBufferNode(visBufferNode **node);

static visVisualResult *visBufferNodeResult(visBufferNode *pNode);

#endif //VISVID_VISBUFFER_H
