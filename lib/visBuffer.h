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
 * @param pRes
 * @param value
 * @param offset
 * @return
 */
int GetVisVisualResultValue(visVisualResult *pRes, uint8_t *value, uint16_t offset);

int SetVisVisualResultData(visVisualResult *pRes, uint8_t *data, size_t length);
#endif //VISVID_VISBUFFER_H
