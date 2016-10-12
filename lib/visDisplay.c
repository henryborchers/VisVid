//
// Created by henry on 10/11/2016.
//

#include <stdint.h>
#include <stdlib.h>
#include "visDisplay.h"

visDisplay *CreateVisDisplay(int width, int height) {
    visDisplay *new_visDisplay = NULL;
    new_visDisplay = (visDisplay*)malloc(sizeof(visDisplay));
    if(new_visDisplay == NULL){
        return NULL;

    }
    new_visDisplay->height = height;
    new_visDisplay->width = width;
    new_visDisplay->data = malloc(sizeof(uint8_t) * width * height);
    return new_visDisplay;
}

void DestroyVisDisplay(visDisplay **pvd) {
    free((*pvd)->data);
    (*pvd)->data = NULL;
    (*pvd)->height = -1;
    (*pvd)->width = -1;
    free(*pvd);
    *pvd = NULL;
}
