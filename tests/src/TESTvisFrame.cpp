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
        pixel = PixelYUV_Create();
    }

    virtual void TearDown() {
        PixelYUV_Destroy(&pixel);
    }

};

struct visFrameYUVFunctions : public ::testing::Test{
    VisYUVFrame *foo;

protected:
    virtual void SetUp() {
        foo = VisYUVFrame_Create();

    }
    virtual void TearDown() {
        VisYUVFrame_Destroy(&foo);
    }


};


TEST(visPixelYUVSetup, visPixelYUV_Create){
    PixelYUV *pixel = NULL;
    pixel = PixelYUV_Create();
    ASSERT_FALSE(pixel == NULL);
}

TEST(visPixelYUVSetup, visPixelYUV_CreateAndCleanup){

    PixelYUV *pixel = NULL;
    pixel = PixelYUV_Create();
    ASSERT_FALSE(pixel == NULL);
    PixelYUV_Destroy(&pixel);
    ASSERT_TRUE(pixel == NULL);
}

TEST_F(visPixelYUVFunctions, setPixel){
    ASSERT_EQ(PixelYUV_setValue(pixel, 4, 100, 40), 0);
}
TEST_F(visPixelYUVFunctions, setPixelBAD){
    PixelYUV *badpix = NULL;
    ASSERT_EQ(PixelYUV_setValue(badpix, 4, 100, 40), EFAULT);
}

TEST_F(visPixelYUVFunctions, setAndGetPixel){
    PixelValue y = 0;
    PixelValue u = 0;
    PixelValue v = 0;
    PixelYUV_setValue(pixel, 4, 100, 40);
    PixelYUV_getValue(pixel, &y, &u, &v);

    ASSERT_EQ(y, 4);
    ASSERT_EQ(u, 100);
    ASSERT_EQ(v, 40);
}

TEST(visFrameSetup, visFrame_Create){
    VisYUVFrame *foo = NULL;
    foo = VisYUVFrame_Create();
    ASSERT_FALSE(foo == NULL);
}

TEST(visFrameSetup, visFrame_CreateAndDestroy){
    VisYUVFrame *foo = VisYUVFrame_Create();
    VisYUVFrame_Destroy(&foo);
    ASSERT_TRUE(foo == NULL);
}

TEST_F(visFrameYUVFunctions, checkEmptyOnCreation){
    int height = 0;
    int width = 0;
    int64_t pos = 0;
    VisYUVFrame_getSize(foo, &width, &height);
    VisYUVFrame_getPos(foo, &pos);

    ASSERT_EQ(height, -1);
    ASSERT_EQ(width, -1);
    ASSERT_EQ(pos, -1);


}

TEST_F(visFrameYUVFunctions, setFrameSize){
    ASSERT_EQ(VisYUVFrame_setSize(foo, 640, 480), 0);
    int height = 0;
    int width = 0;
    VisYUVFrame_getSize(foo, &width, &height);
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

    VisYUVFrame_setSize(foo, width, height);
    ASSERT_EQ(visYUVFrame_Fill(foo, &brush), 0);
    ASSERT_EQ(VisYUVFrame_getPixelYUV(&pix, foo, 50, 50), 0);

    PixelValue y = 0;
    PixelValue u = 0;
    PixelValue v = 0;
    PixelYUV_getValue(&pix, &y, &u, &v);
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


    VisYUVFrame_setSize(foo, width, height);
    for(PixelValue x = 0; x < width; x++){
        brush.Y = x;
        YUVPixel_Draw(foo, &brush, x, 0);
    }

    for(PixelValue x = 0; x < width; x++){
        ASSERT_EQ(VisYUVFrame_getPixelYUV(&pix, foo, x, 0), 0);
        PixelValue y = x;
        PixelValue u = 0;
        PixelValue v = 0;
        PixelYUV_getValue(&pix, &y, &u, &v);
        ASSERT_EQ(y, x);
        ASSERT_EQ(u, brush.U);
        ASSERT_EQ(v, brush.V);
    }



}
