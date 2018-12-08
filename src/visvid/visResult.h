/**
 * @file visResult.h
 * @author Henry Borchers
 * @date 10/11/2016
 */
#ifndef VISVID_VISRESULT_H
#define VISVID_VISRESULT_H

#ifdef MEMORY_DEBUG
#include <stdlib.h>
extern void *d_debug_mem_malloc(size_t size, char *file, size_t line);
extern void *d_debug_mem_calloc(size_t size, size_t num, char *file, size_t line);
#define malloc(size) d_debug_mem_malloc(size, __FILE__, __LINE__)
#define calloc(size, num) d_debug_mem_calloc(size, num, __FILE__, __LINE__)
#endif

#include "visvid/visvid.h"
//#include <stdbool.h>
#include <stddef.h>
//#include "visTypes.h"



/**
 * Creates a new visVisualResult on the heap and returns a pointer to it.
 * @return Pointer to new visVisualResult.
 */
visVisualResult *VisVisualResult_Create();


/**
 * Checks the result has valid data from a calculation
 */
bool VisVisualResult_IsReady(visVisualResult *pRes);

/**
 * Get the value of a visVisualResult at a certain offset.
 * @param value A pointer where the results can be saved.
 * @param pRes The visVisualResult to retrieve the value from.
 * @param offset The index for the value in the data.
 * @return Returns 0 on success.
 */
int VisVisualResult_GetValue(PixelValue *value, visVisualResult *pRes, int offset);



int VisVisualResult_copy(visVisualResult *dst, visVisualResult *src);
#endif //VISVID_VISRESULT_H