//
// Created by henry on 10/11/2016.
//

#include <gtest/gtest.h>

extern "C"{
#include "visView.h"
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
    ASSERT_EQ(visViewRGB_GenerateRGBA(&imageRGB, pvid), 0);

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
