//
// Created by Borchers, Henry Samuel on 10/8/16.
//
#include <stdint.h>
#include <stdlib.h>

#include "visBuffer.h"

/**
 * @struct visBufferNode
 * @brief Wraps the visVisualResult into a linked list.
 */
struct visBufferNode{
    visVisualResult *result;
    visBufferNode *previous;
    visBufferNode *next;
};

typedef struct visBufferNode visBufferNode;
/**
 * @struct visBuffer
 * @brief Used to store the sequence of calculation results from the visualization.
 */
struct visBuffer{
    size_t bufferLen;               /**< Number of nodes in the buffer.*/
    size_t bufferWidth;             /**< The resolution of the calculations*/
    visBufferNode *first;           /**< First node in the buffer*/
    visBufferNode *last;            /**< Last node in the buffer*/
};


visBuffer *CreateVisBuffer(size_t width) {
    visBuffer *buffer = NULL;
    buffer = (visBuffer*)malloc(sizeof(visBuffer));
    if(buffer == NULL){
        return NULL;
    }
    buffer->bufferLen = 0;
    buffer->bufferWidth = width;
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
    // check result size first if it's not null
    if(pRes != NULL){
        // check the size only if there is valid data to check
        if(isVisVisualResultReady(pRes)){
            int resultSize = -1;
            GetVisVisualResultReadySize(pRes, &resultSize);
            if(buffer->bufferWidth != resultSize){
                return -1;
            }
        }
    }
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

size_t visBufferLength(visBuffer *buffer) {
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




