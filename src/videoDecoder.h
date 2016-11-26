//
// Created by Borchers, Henry Samuel on 11/25/16.
//

#ifndef LIBVISVID_VIDEODECODER_H
#define LIBVISVID_VIDEODECODER_H

//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>

typedef struct DecoderContext DecoderContext;

void decoder_init();

DecoderContext *decoderContext_Create(const char* filename);
void decoderContext_Destroy(DecoderContext **pDecoderContext);
int decoderContext_NextFrame(DecoderContext *pDecoderContext, AVFrame **out);


#endif //LIBVISVID_VIDEODECODER_H
