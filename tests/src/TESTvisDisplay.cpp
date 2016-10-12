//
// Created by henry on 10/11/2016.
//

#include <gtest/gtest.h>

extern "C"{
#include "visDisplay.h"
}

TEST(visDisplaySetup, visDisplaySetup_testCreate) {
    visDisplay *pvid = NULL;
    pvid = CreateVisDisplay(640, 480);
    ASSERT_EQ(pvid->width, 640);
    ASSERT_EQ(pvid->height, 480);
    ASSERT_FALSE(pvid == NULL);
}

TEST(visDisplaySetup, visDisplaySetup_testCreateAndDestroy) {
    visDisplay *pvid = CreateVisDisplay(640, 480);
    DestroyVisDisplay(&pvid);
    ASSERT_TRUE(pvid == NULL);
}

class visDisplayFunctions : public ::testing::Test {

protected:
    visDisplay *pvid;

    virtual void SetUp() {
        pvid = CreateVisDisplay(640, 480);
    }

    virtual void TearDown() {
        DestroyVisDisplay(&pvid);
    }


};

TEST_F(visDisplayFunctions, visDisplayFunctions_empty) {
    for(int x = 0; x < pvid->width; x++){
        for(int y = 0; y < pvid->height; y++){
            ASSERT_EQ(pvid->data[x*y], 0);

        }
    }
}


TEST_F(visDisplayFunctions, visDisplayFunctions_emptyBuffer_Test){
    visBuffer *Buffer = CreateVisBuffer(10);
    ASSERT_TRUE(Buffer != NULL);
    int rc = visDisplayUpdate(pvid, Buffer);
    ASSERT_EQ(rc, 0);

    for(int x = 0; x < pvid->width; x++){
        for(int y = 0; y < pvid->height; y++){
            ASSERT_EQ(pvid->data[x*y], 0);

        }
    }
}