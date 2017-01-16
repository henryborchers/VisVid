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

/**
 * @struct visBuffer
 * @brief Used to store the sequence of calculation results from the visualization.
 */
struct visBuffer{
    size_t bufferMaxSize;               /**< Number of nodes in the buffer.*/
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
static int renumber(visBuffer *pBuffer);

static int shift_left(visBuffer *pBuffer);

visBuffer *VisBuffer_Create(size_t width) {
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

int visBuffer_isEmpty(visBuffer *buffer) {
    return buffer->first == NULL;
}


void VisBuffer_Destroy(visBuffer **buffer) {
    if(*buffer){
        while(!visBuffer_isEmpty((*buffer))){
            visBufferNode *node = visBufferPop((*buffer));
            VisBufferNode_Destroy(&node);
        }
        free((*buffer));
        *buffer = NULL;
    }
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

int visBuffer_PushBackResult(visBuffer *buffer, visVisualResult *pRes) {
    // check result size first if it's not null
    if(pRes != NULL){
        // check the size only if there is valid data to check
        if(VisVisualResult_IsReady(pRes)){
            int resultSize = -1;
            VisVisualResult_GetSize(&resultSize, pRes);
            if(buffer->bufferWidth != resultSize){
                return -1;
            }
        }
    }
    // copy the result data into a new node
    visBufferNode *node = NULL;
    node = CreateVisBufferNode(pRes);
    if(NULL == node){
        return -1;
    }
    return visBufferPushBack(buffer, node);
}

int visBufferPushBack(visBuffer *buffer, visBufferNode *newNode) {
    newNode->next = NULL;
    newNode->position = buffer->bufferLen;
    if(visBuffer_isEmpty(buffer)) {
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

visVisualResult *visBuffer_PopResult(visBuffer *buffer) {
    visBufferNode *node = NULL;
    node = visBufferPop(buffer);
    return visBufferNodeResult(node);
}


int renumber(visBuffer *pBuffer) {
    visBufferNode *current = visBufferFront(pBuffer);
    if(NULL == current){
        return 0;
    }
    size_t last_position = current->position;
    current->position = 0;
    while(current){
        if(current->position == 0){
            current = current->next;
            continue;
        }
        current->position = last_position;
        last_position = current->position;
        current = current->next;
    }
    return 0;
}

visVisualResult *visBuffer_PopShiftResult(visBuffer *buffer) {

    visBufferNode   *node = NULL;
    visVisualResult *result = NULL;

    node = visBufferPop(buffer);
    if(NULL == node){
        return NULL;
    }
    result = visBufferNodeResult(node);

    // shift pointers to the data left
    if(renumber(buffer) != 0){
        return NULL;
    } else{

       return result;
    }
}

int shift_left(visBuffer *pBuffer) {
    visBufferNode *front = NULL;
    visBufferNode *current = NULL;
    visBufferNode *next= NULL;

    front = visBufferFront(pBuffer);
    current = front;
    while(current){
        current->position--;
        if(!current->next){
            current->result = front->result;
            break;
        }
        next = visBufferNextNode(current);
        current->result = next->result;
        current = next;
    }
    return 0;
}

visBufferNode * visBufferPop(visBuffer *buffer) {
    visBufferNode *rNode = NULL;

    if(visBuffer_isEmpty(buffer)){
        return NULL;
    }

    rNode = visBufferFront(buffer);

    buffer->first = visBufferNextNode(buffer->first);
//    buffer->first = rNode->next;
    if(buffer->first){

       buffer->first->previous = NULL;
    }
    if(buffer->bufferLen > 0){
        buffer->bufferLen--;
    }

//    Remove any pointers to the return
    rNode->next = NULL;
    rNode->previous = NULL;
    return rNode;
}

size_t visBuffer_getLength(visBuffer *buffer) {
    return buffer->bufferLen;
}

void VisBufferNode_Destroy(visBufferNode **node) {
    if((*node)->result != NULL){
        VisVisualResult_Destroy(&(*node)->result);
    }
    (*node)->result = NULL;
    free(*node);
}

visBufferNode *CreateVisBufferNode(visVisualResult *pRes) {

    visBufferNode *node = NULL;
    node = malloc(sizeof(visBuffer));
    node->previous = NULL;
    node->next = NULL;
    int length = 0;

    // Copy the results data from the original to store in the buffer
    node->result = VisVisualResult_Create();
    VisVisualResult_GetSize(&length, pRes);
    VisVisualResult_SetSize(node->result, length);

    // Only copy the pRes results if they are real and not null
    if(pRes){
        if(VisVisualResult_copy(node->result, pRes) != 0){
            return NULL;
        };
    }
    node->position = 0;
    return node;
}

visVisualResult *visBufferNodeResult(visBufferNode *pNode) {
    return pNode->result;
}

visBufferNode * _BufferNode_get(visBuffer *buffer, size_t index) {
    if(buffer->bufferLen <= index){
        return NULL;
    }

    visBufferNode *n = visBufferFront(buffer);
    if(NULL == n){
        return NULL;
    }
    while(n->position < index){
        n = visBufferNextNode(n);
    }
    if(n->position == index){
        return n;
    }
    return NULL;
}

int _nodePosition(visBufferNode *node) {
    if(NULL == node){
        return -1;
    }
    return (int)node->position;
}

int visBuffer_getResult(PixelValue *pRes, visBuffer *buffer, size_t index) {
    int x;
    visBufferNode *node = NULL;
    node = _BufferNode_get(buffer, index);
    int length = 0;
    if(node == NULL){
        return -1;
    } else {
        if(node->result == NULL){
            return 1;
        }
        VisVisualResult_GetSize(&length, node->result);
        for(x = 0; x < length; x++){
            PixelValue value;
            if(VisVisualResult_GetValue(&value, node->result, x) != 0){
                return -1;
            };
            pRes[x] = value;


        }
        return 0;
    }
    return 1;
}

visBuffer *VisBuffer_Create2(size_t width, size_t bufferSize) {
    visBuffer *buffer = NULL;
    if(bufferSize == 0) {
        buffer = (visBuffer *) malloc(sizeof(visBuffer));
        if (buffer == NULL) {
            return NULL;
        }
        buffer->bufferMaxSize = bufferSize;
        buffer->bufferLen = 0;
        buffer->bufferWidth = width;
        buffer->first = NULL;
        buffer->last = NULL;
        return buffer;
    }

    else {
        return NULL;
    }
}






