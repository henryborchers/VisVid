//
// Created by Borchers, Henry Samuel on 11/22/16.
//

#include <gtest/gtest.h>

extern "C"{
#include "visImageWriter.h"
}

TEST(visImageWriterSetup, visAllocImageRGB_stack){
    visImageRGB imageRGB;
    int height = 100;
    int width = 200;

    visImageRGB_Alloc(&imageRGB, width, height);
    ASSERT_FALSE(imageRGB.plane == NULL);
    ASSERT_EQ(imageRGB.height, height);
    ASSERT_EQ(imageRGB.width, width);
}

TEST(visImageWriterSetup, visAllocImageRGB_heap){
    visImageRGB *imageRGB;
    int height = 100;
    int width = 200;
    imageRGB = (visImageRGB*)malloc(sizeof(visImageRGB));
    ASSERT_FALSE(imageRGB == NULL);
    visImageRGB_Alloc(imageRGB, width, height);
    ASSERT_FALSE(imageRGB->plane == NULL);
    ASSERT_EQ(imageRGB->height, height);
    ASSERT_EQ(imageRGB->width, width);
}

TEST(visImageWriterSetup, visImageRGB_FreeData){
    visImageRGB *imageRGB;
    imageRGB = (visImageRGB*)malloc(sizeof(visImageRGB));
    int height = 100;
    int width = 200;

    visImageRGB_Alloc(imageRGB, width, height);
    visImageRGB_FreeData(imageRGB);
    ASSERT_TRUE(imageRGB->plane == NULL);
    free(imageRGB);

}


class visImageWriterFunctions : public ::testing::Test {

protected:
    visImageRGB *imageRGB;
    const int ALPHA_SHIFT = 0;
    const int BLUE_SHIFT  = 1;
    const int GREEN_SHIFT = 2;
    const int RED_SHIFT   = 3;

    virtual void SetUp(){
        int width = 4;
        int height = 5;



        imageRGB = (visImageRGB*)malloc(sizeof(visImageRGB));
        assert(NULL != imageRGB);
        assert(visImageRGB_Alloc(imageRGB, width, height) == 0);
    }

    virtual void TearDown() {

        free(imageRGB);
    }

};

TEST_F(visImageWriterFunctions, write_red_image) {

    for (int x = 0; x < imageRGB->width; ++x) {
        for (int y = 0; y < imageRGB->height; ++y) {
            visImageRGB_WritePixel(imageRGB, x, y, 255, 0, 0, 255);
        }
    }

    for (int y = 0; y < imageRGB->height; ++y) {
        for (int x = 0; x < imageRGB->width; ++x) {
            int offset = (y * imageRGB->pitch) + (x * sizeof(uint8_t) * 4);
            int value = imageRGB->plane[offset + RED_SHIFT];
            ASSERT_EQ(value, 255);
            ASSERT_EQ(imageRGB->plane[offset + ALPHA_SHIFT], 255);
            ASSERT_EQ(imageRGB->plane[offset + BLUE_SHIFT], 0);
            ASSERT_EQ(imageRGB->plane[offset + GREEN_SHIFT], 0);
        }
    }


}

TEST_F(visImageWriterFunctions, write_ramp_image) {

    for (int x = 0; x < imageRGB->width; ++x) {
        for (int y = 0; y < imageRGB->height; ++y) {
            visImageRGB_WritePixel(imageRGB, x, y, 0, 0, 0, (uint8_t)x);
        }
    }

    for (int y = 0; y < imageRGB->height; ++y) {
        for (int x = 0; x < imageRGB->width; ++x) {
            int offset = (y * imageRGB->pitch) + (x * sizeof(uint8_t) * 4);
            ASSERT_EQ(imageRGB->plane[offset + ALPHA_SHIFT], x);
            ASSERT_EQ(imageRGB->plane[offset + RED_SHIFT], 0);
            ASSERT_EQ(imageRGB->plane[offset + BLUE_SHIFT], 0);
            ASSERT_EQ(imageRGB->plane[offset + GREEN_SHIFT], 0);
        }
    }


}

TEST(visImageRGB_WriteAndReadPixel, blackbox) {
    const int       WIDTH  = 100;
    const int       HEIGHT = 100;

    visImageRGB     image;

    ASSERT_EQ(visImageRGB_Alloc(&image, WIDTH, HEIGHT), 0);
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            uint8_t assigned_red = 0;
            uint8_t returned_red = 10;
            uint8_t assigned_blue = 0;
            uint8_t returned_blue = 10;
            uint8_t assigned_green = 0;
            uint8_t returned_green = 10;
            uint8_t assigned_alpha = 0;
            uint8_t returned_alpha = 10;

            ASSERT_EQ(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green,assigned_blue, assigned_alpha), 0);
            ASSERT_EQ(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue, &returned_alpha), 0);

            ASSERT_EQ(assigned_red, returned_red);
            ASSERT_EQ(assigned_blue, returned_blue);
            ASSERT_EQ(assigned_green, returned_green);
            ASSERT_EQ(assigned_alpha, returned_alpha);

        }
    }

}


TEST(visImageRGB_WriteAndReadPixel, vertical_ramp) {
    const int       WIDTH  = 100;
    const int       HEIGHT = 100;

    visImageRGB     image;

    ASSERT_EQ(visImageRGB_Alloc(&image, WIDTH, HEIGHT), 0);
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            uint8_t assigned_red = y;
            uint8_t returned_red = 10;
            uint8_t assigned_blue = y;
            uint8_t returned_blue = 10;
            uint8_t assigned_green = y;
            uint8_t returned_green = 10;
            uint8_t assigned_alpha = y;
            uint8_t returned_alpha = 10;

            ASSERT_EQ(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green,assigned_blue, assigned_alpha), 0);
            ASSERT_EQ(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue, &returned_alpha), 0);

            ASSERT_EQ(assigned_red, returned_red);
            ASSERT_EQ(assigned_blue, returned_blue);
            ASSERT_EQ(assigned_green, returned_green);
            ASSERT_EQ(assigned_alpha, returned_alpha);

        }
    }

}

TEST(visImageRGB_WriteAndReadPixel, horizontal_ramp) {
    const int       WIDTH = 100;
    const int       HEIGHT = 100;

    visImageRGB     image;

    ASSERT_EQ(visImageRGB_Alloc(&image, WIDTH, HEIGHT), 0);
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            uint8_t assigned_red = x;
            uint8_t returned_red = 10;
            uint8_t assigned_blue = x;
            uint8_t returned_blue = 10;
            uint8_t assigned_green = x;
            uint8_t returned_green = 10;
            uint8_t assigned_alpha = x;
            uint8_t returned_alpha = 10;

            ASSERT_EQ(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green,assigned_blue, assigned_alpha), 0);
            ASSERT_EQ(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue, &returned_alpha), 0);

            ASSERT_EQ(assigned_red, returned_red);
            ASSERT_EQ(assigned_blue, returned_blue);
            ASSERT_EQ(assigned_green, returned_green);
            ASSERT_EQ(assigned_alpha, returned_alpha);

        }
    }

}

TEST(visImageRGB_WriteAndReadPixel, topBottom) {
    const int       WIDTH = 100;
    const int       HEIGHT = 100;

    visImageRGB     image;

    uint8_t assigned_red = 0;
    uint8_t returned_red = 0;
    uint8_t assigned_blue = 0;
    uint8_t returned_blue = 0;
    uint8_t assigned_green = 0;
    uint8_t returned_green = 0;
    uint8_t assigned_alpha = 0;
    uint8_t returned_alpha = 0;

    ASSERT_EQ(visImageRGB_Alloc(&image, WIDTH, HEIGHT), 0);
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            if(y >= 50){
                assigned_red    = 254;
                assigned_blue   = 254;
                assigned_green  = 254;
                assigned_alpha  = 254;

            } else {
                assigned_red    = 0;
                assigned_blue   = 0;
                assigned_green  = 0;
                assigned_alpha  = 0;
            }


            ASSERT_EQ(visImageRGB_WritePixel(&image, x, y, assigned_red, assigned_green,assigned_blue, assigned_alpha), 0);

        }
    }

    for (int x = 0; x < image.width; ++x) {
        for (int y = 0; y < image.height; ++y) {
            ASSERT_EQ(visImageRGB_readPixel(&image, x, y, &returned_red, &returned_green, &returned_blue, &returned_alpha), 0);

            if(y >= 50){
                ASSERT_EQ(254, returned_red);
                ASSERT_EQ(254, returned_blue);
                ASSERT_EQ(254, returned_green);
                ASSERT_EQ(254, returned_alpha);

            } else {

                ASSERT_EQ(0, returned_red);
                ASSERT_EQ(0, returned_blue);
                ASSERT_EQ(0, returned_green);
                ASSERT_EQ(0, returned_alpha);
            }

        }
    }

}