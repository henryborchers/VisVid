//
// Created by henry on 10/11/2016.
//

#include <gtest/gtest.h>

extern "C"{
#include "visvid.h"
}


const int ALPHA_SHIFT = 0;
const int BLUE_SHIFT  = 1;
const int GREEN_SHIFT = 2;
const int RED_SHIFT   = 3;


TEST(visViewSetup, visViewSetup_testCreate) {
    visView *pvid = NULL;
    pvid = VisView_Create(640, 480);
    ASSERT_EQ(pvid->width, 640);
    ASSERT_EQ(pvid->height, 480);
    ASSERT_FALSE(pvid == NULL);
}

TEST(visViewSetup, visViewSetup_testCreateAndDestroy) {
    visView *pvid = VisView_Create(640, 480);
    VisView_Destroy(&pvid);
    ASSERT_TRUE(pvid == NULL);
}

class visViewFunctionsEmptyBuffer : public ::testing::Test {

protected:
    visView *pvid;

    virtual void SetUp() {
        pvid = VisView_Create(640, 480);
    }

    virtual void TearDown() {
        VisView_Destroy(&pvid);
    }


};


TEST_F(visViewFunctionsEmptyBuffer, visViewFunctions_empty) {
    for(int x = 0; x < pvid->width; x++){
        for(int y = 0; y < pvid->height; y++){
            ASSERT_EQ(pvid->data[x + pvid->width * y], 0);

        }
    }
}


TEST_F(visViewFunctionsEmptyBuffer, visViewFunctions_emptyBuffer_Test){
    visBuffer *Buffer = VisBuffer_Create(10);
    ASSERT_TRUE(Buffer != NULL);
    int rc = visView_Update(pvid, Buffer);
    ASSERT_EQ(rc, 0);

    for(int x = 0; x < pvid->width; x++){
        for(int y = 0; y < pvid->height; y++){
            ASSERT_EQ(pvid->data[x + pvid->width * y], 0);

        }
    }
}

class visViewFunctionsFullBuffer1 : public ::testing::Test {

protected:
    visView *pvid;
    visBuffer *buffer;

    virtual void SetUp() {
        buffer= VisBuffer_Create(10);
        pvid = VisView_Create(2, 480);
        visBuffer_PushBackResult(buffer, NULL);
        visBuffer_PushBackResult(buffer, NULL);

    }

    virtual void TearDown() {
        VisView_Destroy(&pvid);
        VisBuffer_Destroy(&buffer);
    }


};

TEST_F(visViewFunctionsFullBuffer1, updateView) {
    ASSERT_EQ(visView_Update(pvid, buffer), 0);
    for(int x = 0; x<pvid->width; x++){
        for(int y = 0; y<pvid->height; y++){
            ASSERT_EQ(pvid->data[x + pvid->width * y], 0);
        }

    };
}

TEST_F(visViewFunctionsFullBuffer1, Create_visViewRGBA) {
    // This test needs to be replaced when color maps are added

    ASSERT_EQ(visView_Update(pvid, buffer), 0);
    visImageRGB imageRGB;

    ASSERT_EQ(visImageRGB_Alloc(&imageRGB, pvid->width, pvid->height), 0);
    ASSERT_EQ(visViewRGB_GenerateRGBA(&imageRGB, pvid, visViewRGBA_value2BW), 0);

    for (int y = 0; y < imageRGB.height; ++y) {
        for (int x = 0; x < imageRGB.width; ++x) {
            int offset = (y * imageRGB.pitch) + (x * sizeof(uint8_t) * 4);
            ASSERT_EQ(imageRGB.plane[offset + ALPHA_SHIFT], 0);
            ASSERT_EQ(imageRGB.plane[offset + RED_SHIFT], 0);
            ASSERT_EQ(imageRGB.plane[offset + BLUE_SHIFT], 0);
            ASSERT_EQ(imageRGB.plane[offset + GREEN_SHIFT], 0);
        }
    }
}

class visViewFunctionsFullBuffer_staticramp : public ::testing::Test {
// Generate a buffer full of result values that go from 0 to 255 vertically.
// Pixel values should remain the same from left to right.
// However, the value of the pixels from the bottom to the top should be a gradient.
// 0 on the top and 255 on the bottom.
protected:
    visView         *pvid;
    visBuffer       *buffer;
    visImageRGB     image;
    const int FRAME_WIDTH = 255;
    const int TOTAL_FRAMES = 255;

//    TODO: visViewFunctionsFullBuffer_ramp test
    virtual void SetUp() {

        // Create a buffer for frames that are 255 pixels wide
        buffer= VisBuffer_Create(FRAME_WIDTH);

        // Create a view that renders to 255 pixels by 255 wide
        pvid = VisView_Create(FRAME_WIDTH, FRAME_WIDTH);

        // Allocate a new image
        visImageRGB_Alloc(&image, FRAME_WIDTH, FRAME_WIDTH);

        // Create a new result container that contains enough info for 255 data points
        for(int i = 0; i < TOTAL_FRAMES; i++){

            visVisualResult *result = NULL;
            result = VisVisualResult_Create();
            assert(0 == VisVisualResult_SetSize(result, FRAME_WIDTH));

            // Set the value from of those data points from 0 to 255.
            PixelValue data_points[FRAME_WIDTH];
            for(PixelValue x = 0; x < FRAME_WIDTH; x++){
                data_points[x] = x;
            }

            assert(data_points[0] == 0);
            assert(data_points[254] == 254);
            VisVisualResult_SetData(result, data_points, FRAME_WIDTH);

            visBuffer_PushBackResult(buffer, result);
        }
    visView_Update(pvid, buffer);
    visViewRGB_GenerateRGBA(&image, pvid, visViewRGBA_value2BW);
    }

    virtual void TearDown() {
        VisView_Destroy(&pvid);
        VisBuffer_Destroy(&buffer);
    }
};

TEST_F(visViewFunctionsFullBuffer_staticramp, alpha) {

    for(int x = 0; x < TOTAL_FRAMES; x++){
        for(int y = 0; y < FRAME_WIDTH; ++y) {
            uint8_t result;
            visImageRGB_readPixel(&image, x, y, NULL, NULL, NULL, &result);
            ASSERT_EQ(result, y) << "Test failed because the value of the pixel's alpha channel in the ramp does not match it's height.";;
        }
    }

}

TEST_F(visViewFunctionsFullBuffer_staticramp, blue) {

    for(int x = 0; x < TOTAL_FRAMES; x++){
        for(int y = 0; y < FRAME_WIDTH; ++y) {
            uint8_t result;
            visImageRGB_readPixel(&image, x, y, NULL, NULL,  &result, NULL);
            ASSERT_EQ(result, y) << "Test failed because the value of the pixel's blue channel in the ramp does not match it's height.";
        }
    }

}

TEST_F(visViewFunctionsFullBuffer_staticramp, red) {

    for(int x = 0; x < TOTAL_FRAMES; x++){
        for(int y = 0; y < FRAME_WIDTH; ++y) {
            uint8_t result;
            visImageRGB_readPixel(&image, x, y, NULL, &result, NULL, NULL);
            ASSERT_EQ(result, y) << "Test failed because the value of the pixel's red channel in the ramp does not match it's height.";
        }
    }

}

TEST_F(visViewFunctionsFullBuffer_staticramp, green) {

    for(int x = 0; x < TOTAL_FRAMES; x++){
        for(int y = 0; y < FRAME_WIDTH; ++y) {
            uint8_t result;
            visImageRGB_readPixel(&image, x, y, &result, NULL, NULL, NULL);
            ASSERT_EQ(result, y) << "Test failed because the value of the pixel's green channel in the ramp does not match it's height.";
        }
    }

}
