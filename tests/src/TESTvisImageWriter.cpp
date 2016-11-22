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

    visAllocImageRGB(&imageRGB, width, height);
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
    visAllocImageRGB(imageRGB, width, height);
    ASSERT_FALSE(imageRGB->plane == NULL);
    ASSERT_EQ(imageRGB->height, height);
    ASSERT_EQ(imageRGB->width, width);
}

TEST(visImageWriterSetup, visFreeImageRGB){
    visImageRGB *imageRGB;
    imageRGB = (visImageRGB*)malloc(sizeof(visImageRGB));
    int height = 100;
    int width = 200;

    visAllocImageRGB(imageRGB, width, height);
    visFreeImageRGB(imageRGB);
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
        assert(visAllocImageRGB(imageRGB, width, height) == 0);
    }

    virtual void TearDown() {

        free(imageRGB);
    }

};

TEST_F(visImageWriterFunctions, write_red_image) {

    for (int x = 0; x < imageRGB->width; ++x) {
        for (int y = 0; y < imageRGB->height; ++y) {
            visImageWritePixelRGB(imageRGB, x, y, 255, 0, 0, 255);
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
            visImageWritePixelRGB(imageRGB, x, y, 0, 0, 0, x);
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
