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
    size_t position;
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

/**
 * Gets the first node of the visBuffer.
 * @param buffer The buffer to find the first node.
 * @return Returns a node in the visBuffer.
 */
static visBufferNode *visBufferFront(visBuffer *buffer);

/**
 * Gets the next node after the given one.
 * @param node The node which to find the next node.
 * @return Returns a pointer to the next node. Returns NULL if none exists.
 */
static visBufferNode *visBufferNextNode(visBufferNode *node);

/**
 * Gets the previous node after the given one.
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
 * Pops a visBufferNode off the front of the a visBuffer.
 * @param buffer The visBuffer to pop a visBufferNode off of.
 * @return Returns a pointer to the visBufferNode that was popped off of the front of the visBuffer.
 */
static visBufferNode *visBufferPop(visBuffer *buffer);

/**
 * Creates a new visBufferNode from a visVisualResult.
 * @param pRes The Result to added to node.
 * @return Returns a new visBufferNode with the visVisualResult.
 */
static visBufferNode *CreateVisBufferNode(visVisualResult *pRes);

/**
 * Get the visVisualResult from a given visBufferNode.
 * @param pNode The visBufferNode to get the visVisualResult from.
 * @return Returns a pointer to visVisualResult.
 */
static visVisualResult *visBufferNodeResult(visBufferNode *pNode);


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
            GetVisVisualResultSize(&resultSize, pRes);
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
    newNode->position = buffer->bufferLen;
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
    node->position = 0;
    return node;
}

visVisualResult *visBufferNodeResult(visBufferNode *pNode) {
    return pNode->result;
}

visBufferNode * _getBufferNode(visBuffer *buffer, size_t index) {
    if(buffer->bufferLen < index){
        return NULL;
    }

    visBufferNode *n = visBufferFront(buffer);

    while(n->position < index){
        n = visBufferNextNode(n);
    }
    if(n->position == index){
        return n;
    }
    return NULL;
}

size_t _nodePosition(visBufferNode *node) {
    return node->position;
}

int getResult(PixelValue *pRes, visBuffer *buffer, size_t index) {
    int x;
    visBufferNode *node = NULL;
    node = _getBufferNode(buffer, index);
    if(node == NULL){
        return -1;
    } else {
        if(node->result == NULL){
            return 0;
        }
        for(x = 0; x < buffer->bufferWidth; x++){
            
            if(GetVisVisualResultValue(pRes, node->result, x) != 0){
                return -1;
            };


        }
    }
    return 1;
}




