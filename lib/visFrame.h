//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#ifndef VISVID_VISFRAME_H_H
#define VISVID_VISFRAME_H_H

typedef struct _PixelYUV PixelYUV;
typedef struct _VisYUVFrame VisYUVFrame;


PixelYUV *CreatePixelYUV();
void DestroyPixelYUV(PixelYUV **pixel);
int setPixelYUV(PixelYUV *pixel, uint8_t y, uint8_t u, uint8_t v);
void getPixelYUV(PixelYUV *pixel, uint8_t *y, uint8_t *u, uint8_t *v);


#endif //VISVID_VISFRAME_H_H
