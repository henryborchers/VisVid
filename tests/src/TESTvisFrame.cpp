//
// Created by Borchers, Henry Samuel on 10/8/16.
//


#include <gtest/gtest.h>

extern "C"{
#include "visFrame.h"
}


using namespace std;
struct visFrameFunctions : public ::testing::Test{
    PixelYUV *pixel;

protected:

    virtual void SetUp() {
        pixel = CreatePixelYUV();
    }

    virtual void TearDown() {
        DestroyPixelYUV(&pixel);
    }

};



TEST(DUMMY, DUMMY_SANIT_Test){
    ASSERT_EQ(1,1);
}

TEST(visFrameSetup, visFrame_Create){
    PixelYUV *pixel = NULL;
    pixel = CreatePixelYUV();
    ASSERT_FALSE(pixel == NULL);
}

TEST(visFrameSetup, visFrame_CreateAndCleanup){

    PixelYUV *pixel = NULL;
    pixel = CreatePixelYUV();
    ASSERT_FALSE(pixel == NULL);
    DestroyPixelYUV(&pixel);
    ASSERT_TRUE(pixel == NULL);
}


TEST_F(visFrameFunctions, setPixel){
    ASSERT_EQ(SetPixelYUV(pixel, 4 , 100, 40), 0);
}

TEST_F(visFrameFunctions, setAndGetPixel){
    uint8_t y = 0;
    uint8_t u = 0;
    uint8_t v = 0;
    SetPixelYUV(pixel, 4 , 100, 40);
    GetPixelYUV(pixel, &y, &u, &v);

    ASSERT_EQ(y, 4);
    ASSERT_EQ(u, 100);
    ASSERT_EQ(v, 40);
}