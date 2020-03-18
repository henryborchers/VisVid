//
// Created by Borchers, Henry Samuel on 10/8/16.
//
#include "visBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Gets the first node of the visBuffer.
 * @param buffer The buffer to find the first node.
 * @return Returns a node in the visBuffer.
 */
static visBufferNode *visBufferFront(const visBuffer *buffer);

/**
 * Gets the next node after the given one.
 * @param node The node which to find the next node.
 * @return Returns a pointer to the next node. Returns NULL if none exists.
 */
static visBufferNode *visBufferNextNode(const visBufferNode *node);

/**
 * Gets the previous node after the given one.
 * @param node The node which to find the previous node.
 * @return Returns a pointer to the previous node. Returns NULL if none exists.
 */
static visBufferNode *visBufferPreviousNode(const visBufferNode *node);

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
static visBufferNode *CreateVisBufferNode(const visVisualResult *pRes);

/**
 * Get the visVisualResult from a given visBufferNode.
 * @param pNode The visBufferNode to get the visVisualResult from.
 * @return Returns a pointer to visVisualResult.
 */
static visVisualResult *visBufferNodeResult(visBufferNode *pNode);

static int renumber(visBuffer *pBuffer);

static int visBuffer_setResult(visBuffer *buffer, size_t index, const visVisualResult *pRes);

static bool VisBuffer_Resize(visBuffer *buffer, size_t width, size_t bufferSize);

int visBuffer_isEmpty(const visBuffer *buffer) {
    return buffer->first == NULL && buffer->last == NULL;
}


void VisBuffer_Destroy(visBuffer **buffer) {
    if (*buffer) {
        while (!visBuffer_isEmpty((*buffer))) {
            visBufferNode *node = visBufferPop((*buffer));
            VisBufferNode_Destroy(&node);
        }
        free((*buffer));
        *buffer = NULL;
    }
}

static visBufferNode *visBufferFront(const visBuffer *buffer) {
    return buffer->first;
}

static visBufferNode *visBufferNextNode(const visBufferNode *node) {
    return node->next;
}

static visBufferNode *visBufferPreviousNode(const visBufferNode *node) {
    return node->previous;
}

int visBuffer_PushBackResult(visBuffer *buffer, const visVisualResult *pRes) {
    int ret = -1;
//    TODO: CHECK IF buffer is unlimited or not. If unlimited, just add, otherwise, shift it left and replace the right most
    // check result size first if it's not null
    if (pRes != NULL) {
        // check the size only if there is valid data to check
        if (VisVisualResult_IsReady(pRes)) {
            signed int resultSize = -1;
            VisVisualResult_GetSize(&resultSize, pRes);
            if (buffer->bufferWidth != (unsigned int)resultSize) {
                return -1;
            }
        }
    }
    // if the buffer is smaller than the max or the buffersize is unlimited, create a new node.
    if (buffer->bufferMaxSize == 0 || buffer->bufferMaxSize > buffer->bufferLen) {
        // copy the result data into a new node
        visBufferNode *node = CreateVisBufferNode(pRes);
        if (NULL == node) {
            return -1;
        }
        return visBufferPushBack(buffer, node);
    }
    // shift the buffer in the data
    if ((ret = visBuffer_ShiftLeft(buffer)) != 0) {
//                TODO clean up
        return ret;
    }

    // use the last one
    if ((ret = visBuffer_setResult(buffer, buffer->bufferLen - 1, pRes)) != 0) {
        return ret;
    }
    return 0;


}

static int visBufferPushBack(visBuffer *buffer, visBufferNode *newNode) {
    newNode->next = NULL;
    newNode->position = buffer->bufferLen;
    if (visBuffer_isEmpty(buffer)) {
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
    visBufferNode *node = visBufferPop(buffer);
    return visBufferNodeResult(node);
}


static int renumber(visBuffer *pBuffer) {
    visBufferNode *current = visBufferFront(pBuffer);
    if (NULL == current) {
        return 0;
    }
    size_t last_position = current->position;
    current->position = 0;
    while (current) {
        if (current->position == 0) {
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

    visVisualResult *result = NULL;

    visBufferNode *node = visBufferPop(buffer);
    if (NULL == node) {
        return NULL;
    }
    result = visBufferNodeResult(node);

    // shift pointers to the data left
    if (renumber(buffer) != 0) {
        return NULL;
    }

    return result;

}

int visBuffer_ShiftLeft(const visBuffer *pBuffer) {
    visBufferNode *next = NULL;
    visVisualResult *first_result = pBuffer->first->result;

    const visBufferNode *front = visBufferFront(pBuffer);
    visBufferNode *current = visBufferFront(pBuffer);
    while (current) {
//        current->position--;
        if (!current->next) {
            current->result = front->result;
            break;
        }
        next = visBufferNextNode(current);
        current->result = next->result;
        current = next;
    }
    pBuffer->last->result = first_result;
    return 0;
}

static visBufferNode *visBufferPop(visBuffer *buffer) {
    visBufferNode *rNode = NULL;

    if (visBuffer_isEmpty(buffer)) {
        return NULL;
    }

    rNode = visBufferFront(buffer);

    if(buffer->first == buffer->last){
        buffer->first = NULL;
        buffer->last = NULL;
    } else {

        buffer->first = visBufferNextNode(buffer->first);
    }
//    buffer->first = rNode->next;
    if (buffer->first) {

        buffer->first->previous = NULL;
    }
    if (buffer->bufferLen > 0) {
        buffer->bufferLen--;
    }

//    Remove any pointers to the return
    rNode->next = NULL;
    rNode->previous = NULL;
    return rNode;
}

size_t visBuffer_getLength(const visBuffer *buffer) {
    return buffer->bufferLen;
}

void VisBufferNode_Destroy(visBufferNode **node) {
    if ((*node)->result != NULL) {
        VisVisualResult_Destroy(&(*node)->result);
    }
    (*node)->result = NULL;
    free(*node);
}

static visBufferNode *CreateVisBufferNode(const visVisualResult *pRes) {

    visBufferNode *node = malloc(sizeof(visBufferNode));
    int length = 0;

    VisVisualResult_GetSize(&length, pRes);

    node->previous = NULL;
    node->next = NULL;

    // Copy the results data from the original to store in the buffer
    node->result = VisVisualResult_Create();
    if(length > 0){
        VisVisualResult_SetSize(node->result, (size_t)length);
    }

    // Only copy the pRes results if they are real and not null
    if (pRes && VisVisualResult_copy(node->result, pRes) != 0) {
        return NULL;
    }
    node->position = 0;
    return node;
}

static visVisualResult *visBufferNodeResult(visBufferNode *pNode) {
    return pNode->result;
}

visBufferNode *_BufferNode_get(const visBuffer *buffer, size_t index) {
    if (buffer->bufferLen <= index) {
        return NULL;
    }

    visBufferNode *n = visBufferFront(buffer);
    if (NULL == n) {
        return NULL;
    }
    while (n->position < index) {
        n = visBufferNextNode(n);
    }
    if (n->position == index) {
        return n;
    }
    return NULL;
}

int _nodePosition(const visBufferNode *node) {
    if (NULL == node) {
        return -1;
    }
    return (int) node->position;
}

int visBuffer_getResult(PixelValue *pRes, const visBuffer *buffer, size_t index) {
    int x;
    const visBufferNode *node = _BufferNode_get(buffer, index);
    int length = 0;

    if (node == NULL) {
        return -1;
    }

    if (node->result == NULL) {
        return 1;
    }

    VisVisualResult_GetSize(&length, node->result);
    for (x = 0; x < length; x++) {
        PixelValue value;
        if (VisVisualResult_GetValue(&value, node->result, x) != 0) {
            return -1;
        }
        pRes[x] = value;


    }
    return 0;


}

visBuffer *VisBuffer_Create2(size_t width, size_t bufferSize) {
    visBuffer *buffer = (visBuffer *) malloc(sizeof(visBuffer));
    bool failed = false;

    if (buffer == NULL) {
        return NULL;
    }

    buffer->bufferMaxSize = bufferSize;
    buffer->bufferWidth = width;
    buffer->first = NULL;
    buffer->last = NULL;
    buffer->bufferLen = 0;
    failed = VisBuffer_Resize(buffer, width, bufferSize);
    buffer->bufferLen = bufferSize;

    if(failed == true){
//        TODO: Clean up this
        return NULL;
    }

    return buffer;
}

static bool VisBuffer_Resize(visBuffer *buffer, size_t width, size_t bufferSize) {
    bool failed = false;//    Automatically resize
    for (unsigned int i = 0; i < bufferSize; i++) {
        visVisualResult *res = VisVisualResult_Create();

        if (NULL == res) {
            failed = true;
            break;
        }

        if (VisVisualResult_SetSize(res, (int) width) != 0) {
            failed = true;
            break;
        }

        if (visBuffer_PushBackResult(buffer, res) != 0) {
            failed = true;
            break;
        }
        VisVisualResult_Destroy(&res);
//        VisVisualResult_Cleanup(res);

    }
    return failed;
}

static int visBuffer_setResult(visBuffer *buffer, size_t index, const visVisualResult *pRes) {
    visBufferNode *node = buffer->first;

//    Find the node
    while (node->position < index) {
        node = node->next;
    }
    if (node->position != index) {
        return -1;
    }
//    Make sure the data will fit
    if (node->result->size != pRes->size) {
        return -1;
    }

//    TODO: set the node's data
    memcpy(node->result->data, pRes->data, sizeof(PixelValue) * node->result->size);
    return 0;
}






