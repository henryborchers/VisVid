//
// Created by henry on 10/11/2016.
//

#include <gtest/gtest.h>

extern "C"{
#include "visView.h"
}

TEST(visViewSetup, visViewSetup_testCreate) {
    visView *pvid = NULL;
    pvid = CreateVisView(640, 480);
    ASSERT_EQ(pvid->width, 640);
    ASSERT_EQ(pvid->height, 480);
    ASSERT_FALSE(pvid == NULL);
}

TEST(visViewSetup, visViewSetup_testCreateAndDestroy) {
    visView *pvid = CreateVisView(640, 480);
    DestroyVisView(&pvid);
    ASSERT_TRUE(pvid == NULL);
}

class visViewFunctionsEmptyBuffer : public ::testing::Test {

protected:
    visView *pvid;

    virtual void SetUp() {
        pvid = CreateVisView(640, 480);
    }

    virtual void TearDown() {
        DestroyVisView(&pvid);
    }


};


TEST_F(visViewFunctionsEmptyBuffer, visViewFunctions_empty) {
    for(int x = 0; x < pvid->width; x++){
        for(int y = 0; y < pvid->height; y++){
            ASSERT_EQ(pvid->data[x*y], 0);

        }
    }
}


TEST_F(visViewFunctionsEmptyBuffer, visViewFunctions_emptyBuffer_Test){
    visBuffer *Buffer = CreateVisBuffer(10);
    ASSERT_TRUE(Buffer != NULL);
    int rc = visViewUpdate(pvid, Buffer);
    ASSERT_EQ(rc, 0);

    for(int x = 0; x < pvid->width; x++){
        for(int y = 0; y < pvid->height; y++){
            ASSERT_EQ(pvid->data[x*y], 0);

        }
    }
}

class visViewFunctionsFullBuffer1 : public ::testing::Test {

protected:
    visView *pvid;
    visBuffer *buffer;

    virtual void SetUp() {
        buffer= CreateVisBuffer(10);
        pvid = CreateVisView(2, 480);
        visBufferPushBackResult(buffer, NULL);
        visBufferPushBackResult(buffer, NULL);

    }

    virtual void TearDown() {
        DestroyVisView(&pvid);
        DestroyVisBuffer(&buffer);
    }


};

TEST_F(visViewFunctionsFullBuffer1, updateView) {
    ASSERT_EQ(visViewUpdate(pvid, buffer), 0);
    for(int x = 0; x<pvid->width; x++){
        for(int y = 0; y<pvid->height; y++){
            ASSERT_EQ(pvid->data[x*y], 0);
        }

    };
}
