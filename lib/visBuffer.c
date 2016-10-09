//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "visBuffer.h"

struct _visBufferNode{
    visVisualResult *result;
    visBufferNode *previous;
    visBufferNode *next;
};

struct _visBuffer{
    size_t bufferLen;
    visBufferNode *first;
    visBufferNode *last;
};


struct _visVisualResult{
    bool ready;
    int size;
    uint8_t *data;
};

visVisualResult *CreateVisVisualResult() {
    visVisualResult *newResult = NULL;
    newResult = (visVisualResult*)malloc(sizeof(visVisualResult));
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
    pRest->data = calloc((size_t )size, sizeof(uint8_t));
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

int GetVisVisualResultValue(visVisualResult *pRes, uint8_t *value, uint16_t offset) {
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
    memcpy(pRes->data, data, sizeof(uint8_t) * length);
    pRes->ready = true;
    return 0;
}

visBuffer *CreateVisBuffer() {
    visBuffer *buffer = NULL;
    buffer = (visBuffer*)malloc(sizeof(visBuffer));
    if(buffer == NULL){
        return NULL;
    }
    buffer->bufferLen = 0;
    buffer->first = NULL;
    buffer->last = NULL;
    return buffer;
}

int visBufferIsEmpty(visBuffer *buffer) {
    return buffer->first == NULL;
}


void DestroyVisBuffer(visBuffer **buffer) {
    while(!visBufferIsEmpty((*buffer))){
        visBufferNode *node = visBufferPop((*buffer));
        DestroyVisBufferNode(&node);
    }
    free((*buffer));
    *buffer = NULL;
}

visBufferNode *visBufferFront(visBuffer *buffer) {
    return buffer->first;
}

visBufferNode *visBufferNextNode(visBufferNode *node) {
    return node->next;
}

visBufferNode *visBufferPreviousNode(visBufferNode *node) {
    return node->previous;
}

int visBufferPushBackResult(visBuffer *buffer, visVisualResult *pRes) {
    visBufferNode *node = NULL;
    node = CreateVisBufferNode(pRes);
    return visBufferPushBack(buffer, node);
}

int visBufferPushBack(visBuffer *buffer, visBufferNode *newNode) {
    newNode->next = NULL;
    if(visBufferIsEmpty(buffer)) {
        newNode->previous = NULL;
        buffer->first = newNode;
        buffer->last = newNode;
    } else {
        newNode->previous = buffer->last;
        buffer->last->next = newNode;
        buffer->last = newNode;
    }
    buffer->bufferLen++;
    return 0;
}

visVisualResult *visBufferPopResult(visBuffer *buffer) {
    visBufferNode *node = NULL;
    node = visBufferPop(buffer);
    return visBufferNodeResult(node);
}

visBufferNode *visBufferPop(visBuffer *buffer) {
    visBufferNode *rNode = NULL;

    if(visBufferIsEmpty(buffer)){
        return NULL;
    }

    rNode = visBufferFront(buffer);

    buffer->first = visBufferNextNode(rNode);
    buffer->first = rNode->next;
    buffer->bufferLen--;

    rNode->next = NULL;
    rNode->previous = NULL;
    return rNode;
}

size_t visBufferSize(visBuffer *buffer) {
    return buffer->bufferLen;
}

void DestroyVisBufferNode(visBufferNode **node) {
    if((*node)->result != NULL){
        DestroyVisVisualResult(&(*node)->result);
    }
    (*node)->result = NULL;
    free(*node);
}

visBufferNode *CreateVisBufferNode(visVisualResult *pRes) {
    visBufferNode *node = NULL;
    node = malloc(sizeof(visBuffer));
    node->previous = NULL;
    node->next = NULL;
    node->result = pRes;
    return node;
}

visVisualResult *visBufferNodeResult(visBufferNode *pNode) {
    return pNode->result;
}




