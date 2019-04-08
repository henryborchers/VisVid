//
// Created by henry on 10/11/2016.
//

#include <visBuffer.h>
#include <visvid/utils.h>
#include <visvid/visvid.h>
#include <libavutil/pixdesc.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include "visView.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_COLORS 4

static float min(float d, float d1);



visView *VisView_Create(int width, int height) {
    visView *new_visView = NULL;
    new_visView = (visView *) malloc(sizeof(visView));
    if (new_visView == NULL) {
        return NULL;

    }
    new_visView->height = height;
    new_visView->width = width;

    new_visView->data = calloc((size_t) (width * height), sizeof(PixelValue));
    return new_visView;
}

void VisView_Destroy(visView **pvd) {
    free((*pvd)->data);
    (*pvd)->data = NULL;
    (*pvd)->height = -1;
    (*pvd)->width = -1;
    free(*pvd);
    *pvd = NULL;
}


int visView_Update2(visView *pView, visBuffer *buffer) {
    int x;
    int y;
    int res = 0;

    PixelValue currentSlice[pView->width];


    for (x = 0; x < pView->width; x++) {
        res = visBuffer_getResult(currentSlice, buffer, x);
        if (res == 0) {

            for (y = 0; y < pView->height; ++y) {
                pView->data[x + pView->width * y] = currentSlice[y];

            }
        } else {
            for (y = 0; y < pView->height; ++y) {

                pView->data[x + pView->width * y] = 0;

            }
        }


    }
    return 0;
}

int visView_Update3(visView *pView, visBuffer *buffer) {
    int x;
    int y;
    int res = 0;

    PixelValue currentSlice[pView->width];


    for (x = 0; x < pView->width; x++) {
        size_t length = visBuffer_getLength(buffer);
        res = visBuffer_getResult(currentSlice, buffer, length - x);
        if (res == 0) {

            for (y = 0; y < pView->height; ++y) {
                pView->data[pView->width - x + pView->width * y] = currentSlice[y];

            }
        } else {
            for (y = 0; y < pView->height; ++y) {

                pView->data[pView->width - x + pView->width * y] = 0;

            }
        }


    }
    return 0;
}

int visView_Update4(visView *pView, visBuffer *buffer) {
    if(pView == NULL || buffer == NULL){
        return -1;
    }
    PixelValue currentSlice[pView->width];
    size_t length = visBuffer_getLength(buffer);
    for (int y = 0; y < pView->height; ++y) {
            int res = visBuffer_getResult(currentSlice, buffer, buffer->bufferLen - 1 - y);
            if(res != 0 ){
                return res;
            }
            int offset =  (pView->width * y);
            PixelValue *data = pView->data + offset;
            memcpy(data, currentSlice, pView->width);
    }
    return 0;
}


int index_lookup(int x, int y, int view_width){
//    int index = (view_width * (y-1)) + (view_width - x);
    int index = (view_width * y) + x;
    return index;
}

//int visView_Update4(visView *pView, visBuffer *buffer) {
//    int x;
//    int y;
//    int res = 0;
//
//    PixelValue currentSlice[pView->width];
//
//
//    for (x = 0; x < pView->width; x++) {
//        size_t length = visBuffer_getLength(buffer);
//        res = visBuffer_getResult(currentSlice, buffer, length - x - 1);
//        if (res == 0) {
//
//            for (y = 0; y < pView->height; ++y) {
//                int index = index_lookup(x, y, pView->width);
////                int index = ((pView->width * y) + (pView->width - x - 1)) - 1;
////                int index = (pView->width * (y-1)) + (pView->width - x);
//                PixelValue value = currentSlice[y];
////                int index = (pView->width - x) + pView->width * y;
//                pView->data[index] = value;
//
//            }
//        } else {
//            for (y = 0; y < pView->height; ++y) {
//                int index = index_lookup(x, y, pView->width);
////                int index = (pView->width * (y-1)) + (pView->width - x);
////                int index = (pView->width * y) + (pView->width - x - 1);
//                pView->data[index] = 0;
//
//            }
//        }
//
//
//    }
//    return 0;
//}


// FIXME visView_Update is broken
// depricated
int visView_Update(visView *pView, visBuffer *buffer) {
    int x;
    int y;
    int valid_result = 0;
    size_t buffersize = visBuffer_getLength(buffer);
//    int starting = buffersize - pView->width;
//    if(starting < 0){
//        starting = 0;
//    }
//    int ending = starting + pView->width;

    PixelValue currentSlice[pView->height];
    PixelValue lastSlice[pView->height];

    for (x = 0; x < buffersize; x++) {
        // If the result is null the result code will be 0
        valid_result = visBuffer_getResult(currentSlice, buffer, x);
//        printf("value of x is %d\n", x);
        if (valid_result < 0) {

            fprintf(stderr, "Invalid result\n");
            return valid_result;
        };
        for (y = 0; y < pView->width; y++) {
            // If the result is a true result, the memory can be copied
            if (valid_result == 1) {
                pView->data[x + pView->width * y] = currentSlice[y];
            } else {
//                fprintf(stderr, "Invalid data\n");
                // If the result isn't a true result (for example: there is no data calculated yet), do one of the following.
                // If it's the first one, render it out as black.
                if (x == 0) {
                    pView->data[x + pView->width * y] = 0;
                    currentSlice[y] = 0;
                } else {
                    // Otherwise, use the last good slice
                    pView->data[x + pView->width * y] = lastSlice[y];
                }
            }
        }
        if (valid_result || x == 0) {
            memcpy(lastSlice, currentSlice, pView->height);
        }
    }
    return 0;
}


int visViewRGB_GenerateRGBA(visImageRGB *out, visView *pView,
                            int (*callback)(PixelValue, uint8_t *, uint8_t *, uint8_t *, uint8_t *)) {

    // assert that neither pointer is NULL.

    if (NULL == out || NULL == pView) {
        return EFAULT;
    }

    // Verify that the image is the same size as the view

    if (out->width != pView->width) {
        return -1;
    }

    if (out->height != pView->height) {
        return -1;
    }

    // Render the data
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
    for (int y = 0; y < out->height; ++y) {
        for (int x = 0; x < out->width; ++x) {
            callback(pView->data[x + pView->width * y], &r, &g, &b, &a);
            visImageRGB_WritePixel(out, x, y, r, g, b, a);
        }
    }
    return 0;
}

int visViewRGBA_value2BW(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {
    *r = value;
    *g = value;
    *b = value;
    *a = value;
    return 0;
}

float min(float d, float d1) {

    return (d < d1) ? d : d1;;
}

int visViewRGBA_value2color1(PixelValue value, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {
    // FIXME: REPLACE WITH ORIGINAL FUNCTION
    // This is temp code that was modified from a web tutorial. Replace this with an original callback function.
    float i = (float) value / (float) 255;

    static float color[NUM_COLORS][3] = {{0, 0,   0},
                                         {1, 0.2, 0.2},
                                         {1, 1,   0.2},
                                         {1, 1,   1}};
    // A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.

    int idx1;        // |-- Our desired color will be between these two indexes in "color".
    int idx2;        // |
    float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.

    if (i <= 0) {
        idx1 = idx2 = 0;                // accounts for an input <=0
    } else if (i >= 1) {
        idx1 = idx2 = NUM_COLORS - 1;   // accounts for an input >=0
    } else {
        i = i * (NUM_COLORS - 1);        // Will multiply value by 3.
        idx1 = (int) floorf(i);                  // Our desired color will be after this index.
        idx2 = idx1 + 1;                        // ... and before this index (inclusive).
        fractBetween = i - (float) (idx1);    // Distance between the two indexes (0-1).
    }

    *r = (uint8_t) (value * min(1.0f, (color[idx2][0] - color[idx1][0]) * fractBetween + color[idx1][0]));
    *g = (uint8_t) (value * min(1.0f, (color[idx2][1] - color[idx1][1]) * fractBetween + color[idx1][1]));
    *b = (uint8_t) (value * min(1.0f, (color[idx2][2] - color[idx1][2]) * fractBetween + color[idx1][2]));
    *a = value;
    return 0;
}

size_t calculateImageBWOffset(int x, int y, int width, int num_pix_components){
    int source_line_offset = y * width;
    int pixel_offset = x * num_pix_components;
    return source_line_offset + pixel_offset;
}

int visView_GenerateBW(visImage *pImage, visView *pView) {
//    FIGURE OUT the line offset of the view
    for (int y = 0; y < pImage->height; ++y) {
        for (int x = 0; x < pImage->width; ++x) {
            int offset = calculateImageBWOffset(x, y, pImage->width, pImage->num_pix_components);
            int view_line_offset = pView->width * y;
            int view_offset = view_line_offset + x;
            PixelValue value =  pView->data[view_offset];
            pImage->plane[offset] = value;

        }
    }
    return 0;
}