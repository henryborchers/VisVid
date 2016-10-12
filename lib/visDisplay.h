//
// Created by henry on 10/11/2016.
//

#pragma once
typedef struct {
    int width;
    int height;
    uint8_t *data;
}visDisplay;

visDisplay *CreateVisDisplay(int width, int height);

void DestroyVisDisplay(visDisplay **pvd);