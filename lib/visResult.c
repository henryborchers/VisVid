//
// Created by henry on 10/11/2016.
//

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include "visResult.h"

struct visVisualResult{
    bool ready;
    int size;
    unsigned char *data;
};

visVisualResult *CreateVisVisualResult() {
    visVisualResult *newResult = NULL;
    newResult = (visVisualResult*) malloc(sizeof(visVisualResult));
    newResult->ready = false;
    newResult->size = -1;
    newResult->data = NULL;
    return newResult;
}

void DestroyVisVisualResult(visVisualResult **pRes) {
    free((*pRes)->data);

    (*pRes)->data = NULL;
    (*pRes)->size = -1;
    (*pRes)->ready = false;
    free((*pRes));
    *pRes = NULL;

}

bool isVisVisualResultReady(visVisualResult *pRes) {
    return pRes->ready;
}

int SetVisVisualResultReadySize(visVisualResult *pRest, int size) {
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

int GetVisVisualResultReadySize(visVisualResult *pRest, int *size) {
    if(pRest == NULL){
        return EFAULT;
    }
    *size = pRest->size;
    return 0;
}

int GetVisVisualResultValue(visVisualResult *pRes, uint8_t *value, unsigned short offset) {
    if(value == NULL){
        return EFAULT;
    }
    if(offset > pRes->size){
        return EFAULT;
    }
    *value = pRes->data[offset];
    return 0;
}

int SetVisVisualResultData(visVisualResult *pRes, uint8_t *data, size_t length) {
    if(pRes->size != length){
        return EFAULT;
    }
    memcpy(pRes->data, data, sizeof(unsigned char) * length);
    pRes->ready = true;
    return 0;
}