//
// Created by Borchers, Henry Samuel on 10/22/16.
//
/**
 * @file visVisualization.h
 * @author Henry Borchers
 * @date October 22, 2016
 * @brief Contains all the function for calculating the visualizations.
 */
#ifndef VISVID_VISVISUALIZATION_H
#define VISVID_VISVISUALIZATION_H

#ifdef MEMORY_DEBUG
#include <stdlib.h>
//extern void *d_debug_mem_malloc(size_t size, char *file, size_t line);
//extern void *d_debug_mem_calloc(size_t size, size_t num, char *file, size_t line);
//#define malloc(size) d_debug_mem_malloc(size, __FILE__, __LINE__)
//#define calloc(size, num) d_debug_mem_calloc(size, num, __FILE__, __LINE__)
#endif

#include "visvid/visvid.h"
#include "visvid/utils.h"

#include "visResult.h"
#include "visFrame.h"
#endif //VISVID_VISVISUALIZATION_H