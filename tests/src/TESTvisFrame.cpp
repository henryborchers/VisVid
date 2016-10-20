//
// Created by Borchers, Henry Samuel on 10/8/16.
//


#include <gtest/gtest.h>

extern "C"{
#include "visFrame.h"
}


using namespace std;
struct visPixelYUVFunctions : public ::testing::Test{
    PixelYUV *pixel;

protected:

    virtual void SetUp() {
        pixel = CreatePixelYUV();
    }

    virtual void TearDown() {
        DestroyPixelYUV(&pixel);
    }

};

struct visFrameYUVFunctions : public ::testing::Test{
    VisYUVFrame *foo;

protected:
    virtual void SetUp() {
        foo = CreateVisYUVFrame();

    }
    virtual void TearDown() {
        DestroyVisYUVFrame(&foo);
    }


};


TEST(visPixelYUVSetup, visPixelYUV_Create){
    PixelYUV *pixel = NULL;
    pixel = CreatePixelYUV();
    ASSERT_FALSE(pixel == NULL);
}

TEST(visPixelYUVSetup, visPixelYUV_CreateAndCleanup){

    PixelYUV *pixel = NULL;
    pixel = CreatePixelYUV();
    ASSERT_FALSE(pixel == NULL);
    DestroyPixelYUV(&pixel);
    ASSERT_TRUE(pixel == NULL);
}

TEST_F(visPixelYUVFunctions, setPixel){
    ASSERT_EQ(SetPixelYUV(pixel, 4 , 100, 40), 0);
}

TEST_F(visPixelYUVFunctions, setAndGetPixel){
    PixelValue y = 0;
    PixelValue u = 0;
    PixelValue v = 0;
    SetPixelYUV(pixel, 4 , 100, 40);
    GetPixelYUV(pixel, &y, &u, &v);

    ASSERT_EQ(y, 4);
    ASSERT_EQ(u, 100);
    ASSERT_EQ(v, 40);
}

TEST(visFrameSetup, visFrame_Create){
    VisYUVFrame *foo = NULL;
    foo = CreateVisYUVFrame();
    ASSERT_FALSE(foo == NULL);
}

TEST(visFrameSetup, visFrame_CreateAndDestroy){
    VisYUVFrame *foo = CreateVisYUVFrame();
    DestroyVisYUVFrame(&foo);
    ASSERT_TRUE(foo == NULL);
}

TEST_F(visFrameYUVFunctions, checkEmptyOnCreation){
    int height = 0;
    int width = 0;
    GetVisYUVFrameSize(foo, &width, &height);

    ASSERT_EQ(height, -1);
    ASSERT_EQ(width, -1);
}

TEST_F(visFrameYUVFunctions, setFrameSize){
    ASSERT_EQ(SetVisYUVFrameSize(foo, 640, 480), 0);
    int height = 0;
    int width = 0;
    GetVisYUVFrameSize(foo, &width, &height);
    ASSERT_EQ(height, 480);
    ASSERT_EQ(width, 640);
}