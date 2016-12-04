//
// Created by Borchers, Henry Samuel on 11/25/16.
//

#ifndef LIBVISVID_VIDEODECODER_H
#define LIBVISVID_VIDEODECODER_H
#include <libavutil/frame.h>

typedef struct DecoderContext DecoderContext;

void decoder_init();

DecoderContext *decoderContext_Create(const char* filename);
void decoderContext_Destroy(DecoderContext **pDecoderContext);
int decoderContext_NextFrame(DecoderContext *pDecoderContext, AVFrame **out);
int decoderContext_Rewind(DecoderContext *pDecoderContext);
void decoderContext_GetSize(DecoderContext *pDecoderContext, int *width, int *height);

#endif //LIBVISVID_VIDEODECODER_H
