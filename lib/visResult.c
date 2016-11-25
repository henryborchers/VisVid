//
// Created by henry on 10/11/2016.
//

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include "visResult.h"


/**
 * @struct visVisualResult
 * @brief  Contains the calculated data from a visualization calculation.
 */
struct visVisualResult{
    bool ready;         /**< Declares if a result has been fully calculated.*/
    int size;           /**< The number of pixels wide the result is. */
    PixelValue *data;   /**< Raw Data */
};

visVisualResult *VisVisualResult_Create() {
    visVisualResult *newResult = NULL;
    newResult = (visVisualResult*) malloc(sizeof(visVisualResult));
    newResult->ready = false;
    newResult->size = -1;
    newResult->data = NULL;
    return newResult;
}

void VisVisualResult_Destroy(visVisualResult **pRes) {
    free((*pRes)->data);

    (*pRes)->data = NULL;
    (*pRes)->size = -1;
    (*pRes)->ready = false;
    free((*pRes));
    *pRes = NULL;

}

bool VisVisualResult_IsReady(visVisualResult *pRes) {
    return pRes->ready;
}

int VisVisualResult_SetSize(visVisualResult *pRest, int size) {
    if(pRest == NULL){
        return EFAULT;
    }
    if(pRest->size > 0) {
        free(pRest->data);
    }
    pRest->data = NULL;
    pRest->data = calloc((unsigned long long int)size, sizeof(unsigned char));
    if(pRest->data == NULL){
        pRest->size = -1;
        pRest->ready = false;
        return ENOMEM;
    }
    pRest->size = size;
    return 0;
}

int VisVisualResult_GetSize(int *size, visVisualResult *pRest) {
    if(pRest == NULL){
        return EFAULT;
    }
    *size = pRest->size;
    return 0;
}

int VisVisualResult_GetValue(PixelValue *value, visVisualResult *pRes, int offset) {
    if(value == NULL){
        return EFAULT;
    }
    if(pRes == NULL){
        return 0;
    }
    if(offset > pRes->size){
        return EFAULT;
    }
    *value = pRes->data[offset];
    return 0;
}

int VisVisualResult_SetData(visVisualResult *pRes, PixelValue *data, size_t length) {
    if(data == NULL){
        return EFAULT;
    }
    if(pRes->size != length){
        return EFAULT;
    }
    memcpy(pRes->data, data, sizeof(PixelValue) * length);
    pRes->ready = true;
    return 0;
}