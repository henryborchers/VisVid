//
// Created by henry on 4/14/19.
//

#include "pgm.h"
#include "stdio.h"

/**
 * Save visImage as a pgm image to a file.
 * @param source image data
 * @param filename output name to use when saving to a file
 */
void image_save_pgm(const visImage *image, const char *filename)
{
    FILE *f;
    int i;
    unsigned char *buf = image->plane;
    int wrap = image->width;
    int xsize = image->width;
    int ysize = image->height;


    f = fopen(filename,"w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}

