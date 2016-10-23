//
// Created by Borchers, Henry Samuel on 10/8/16.
//


#include <gtest/gtest.h>

extern "C"{
#include "visFrame.h"
}


using namespace std;

//
//TEST(visFrame, version) {
//    EXPECT_EQ(VISFRAME_VERSION, VISFRAME_VERSION);
//}

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
    ASSERT_EQ(SetPixelValueYUV(pixel, 4, 100, 40), 0);
}
TEST_F(visPixelYUVFunctions, setPixelBAD){
    PixelYUV *badpix = NULL;
    ASSERT_EQ(SetPixelValueYUV(badpix, 4, 100, 40), EFAULT);
}

TEST_F(visPixelYUVFunctions, setAndGetPixel){
    PixelValue y = 0;
    PixelValue u = 0;
    PixelValue v = 0;
    SetPixelValueYUV(pixel, 4, 100, 40);
    GetPixelValueYUV(pixel, &y, &u, &v);

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
    int64_t pos = 0;
    GetVisYUVFrameSize(foo, &width, &height);
    GetVisYUVFrameSizePos(foo, &pos);

    ASSERT_EQ(height, -1);
    ASSERT_EQ(width, -1);
    ASSERT_EQ(pos, -1);


}

TEST_F(visFrameYUVFunctions, setFrameSize){
    ASSERT_EQ(SetVisYUVFrameSize(foo, 640, 480), 0);
    int height = 0;
    int width = 0;
    GetVisYUVFrameSize(foo, &width, &height);
    ASSERT_EQ(height, 480);
    ASSERT_EQ(width, 640);
}

TEST_F(visFrameYUVFunctions, BrushFill) {
    int height = 100;
    int width = 200;
    visBrush brush;
    brush.Y = 20;
    brush.U = 30;
    brush.V = 40;
    PixelYUV pix;

    SetVisYUVFrameSize(foo, width, height);
    ASSERT_EQ(visFillYUVFrame(foo, &brush), 0);
    ASSERT_EQ(GetPixelFromYUVFrame(&pix, foo, 50, 50), 0);

    PixelValue y = 0;
    PixelValue u = 0;
    PixelValue v = 0;
    GetPixelValueYUV(&pix, &y, &u, &v);
    ASSERT_EQ(y, brush.Y);
    ASSERT_EQ(u, brush.U);
    ASSERT_EQ(v, brush.V);

}

TEST_F(visFrameYUVFunctions, BrushDraw) {
    int height = 20;
    int width = 100;
    visBrush brush;
    brush.Y = 20;
    brush.U = 30;
    brush.V = 40;
    PixelYUV pix;


    SetVisYUVFrameSize(foo, width, height);
    for(PixelValue x = 0; x < width; x++){
        brush.Y = x;
        visDrawYUVPixel(foo, &brush, x, 0);
    }

    for(PixelValue x = 0; x < width; x++){
        ASSERT_EQ(GetPixelFromYUVFrame(&pix, foo, x, 0), 0);
        PixelValue y = x;
        PixelValue u = 0;
        PixelValue v = 0;
        GetPixelValueYUV(&pix, &y, &u, &v);
        ASSERT_EQ(y, x);
        ASSERT_EQ(u, brush.U);
        ASSERT_EQ(v, brush.V);
    }



}
