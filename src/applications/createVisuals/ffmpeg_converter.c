//
// Created by Borchers, Henry Samuel on 12/17/16.
//

#include "ffmpeg_converter.h"
#include <libavutil/frame.h>
#include <libavutil/pixdesc.h>
#include <stdint.h>
#include "visFrame.h"
#include "visTypes.h"

int ffmpeg2visframe(VisYUVFrame *dst, struct AVFrame *src) {
    int res;

    AVPixFmtDescriptor *desc = (AVPixFmtDescriptor *) av_pix_fmt_desc_get(src->format);
    for(int y = 0; y < src->height; y++){
        for(int x = 0; x < src->width; x++){

           // TODO: Make this API better so I don't have to use the stupid brush and draw function. It was a really a dumb idea
            visBrush brush;
            int uvx = x >> desc->log2_chroma_w;
            int uvy = y >> desc->log2_chroma_h;

            brush.Y = (PixelValue)src->data[0][src->linesize[0] * y + x];
            brush.U = (PixelValue)src->data[1][src->linesize[1] * uvy + uvx];
            brush.V = (PixelValue)src->data[2][src->linesize[2] * uvy + uvx];
            if((res = YUVPixel_Draw(dst, &brush, x, y)) != 0){
                return res;
            };

        }
    }
    return 0;
}
