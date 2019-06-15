//
// Created by henry on 10/11/2016.
//

#include "visResult.h"
#include <errno.h>
//#include <memory.h>
#include <string.h>
#include <stdlib.h>



visVisualResult *VisVisualResult_Create() {
    visVisualResult *newResult = (visVisualResult*) malloc(sizeof(visVisualResult));
    VisVisualResult_Init(newResult);
    return newResult;
}

int VisVisualResult_Init(visVisualResult *newResult) {
    newResult->ready = false;
    newResult->size = -1;
    newResult->data = NULL;
    return 0;
}

void VisVisualResult_Destroy(visVisualResult **pRes) {
    VisVisualResult_Cleanup(*pRes);

    free((*pRes));
    *pRes = NULL;

}

void VisVisualResult_Cleanup(visVisualResult *pRes) {
    free(pRes->data);

    (pRes)->data = NULL;
    (pRes)->size = -1;
    (pRes)->ready = false;
}

bool VisVisualResult_IsReady(const visVisualResult *pRes) {
    return pRes->ready;
}

int VisVisualResult_SetSize(visVisualResult *pRest, size_t size) {
    if(pRest == NULL){
        return EFAULT;
    }

//    if(pRest->size > 0) {
//        free(pRest->data);
//    }

//    pRest->data = NULL;
    size_t data_size = size * sizeof(unsigned char);
    pRest->data = realloc(pRest->data, data_size);
    if(pRest->data == NULL){
        pRest->size = -1;
        pRest->ready = false;
        return ENOMEM;
    }
    memset(pRest->data, 0, data_size);
    pRest->size = (int)size;
    return 0;
}

int VisVisualResult_GetSize(int *size, const visVisualResult *pRest) {
    if(pRest == NULL){
        return EFAULT;
    }
    *size = pRest->size;
    return 0;
}

int VisVisualResult_GetValue(PixelValue *value, const visVisualResult *pRes, int offset) {
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

int VisVisualResult_SetData(visVisualResult *pRes, const PixelValue *data, size_t length) {
    if(data == NULL){
        return EFAULT;
    }
    if(pRes->size != length){
        return EFAULT;
    }
    pRes->data = memcpy(pRes->data, data, sizeof(PixelValue) * length);
    pRes->ready = true;
    return 0;
}

int VisVisualResult_copy(visVisualResult *dst, const visVisualResult *src) {
    if(NULL == src){
        return 1;
    }

    if(src == dst){
        return -2;
    }
    if(src->size != dst->size){
        return -1;

    }
    if(NULL == dst->data && NULL == src->data){

    } else{
        memcpy(dst->data, src->data, sizeof(PixelValue) * src->size);
    }
    dst->ready = src->ready;
    return 0;
}
