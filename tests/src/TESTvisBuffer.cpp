//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <gtest/gtest.h>
extern "C"{
#include "visBuffer.h"
}


TEST(visBufferSetup, visBufferCreate){
    visBuffer *buffer = NULL;
    buffer = CreateVisBuffer(0);
    ASSERT_FALSE(buffer == NULL);
}
TEST(visBufferSetup, visBufferCreateAndDestroy){
    visBuffer *buffer = CreateVisBuffer(0);
    DestroyVisBuffer(&buffer);
    ASSERT_TRUE(buffer == NULL);
}

class visBufferFunctions : public ::testing::Test {

protected:
    visBuffer *buffer;
    virtual void SetUp() {
        buffer = CreateVisBuffer(0);
    }

    virtual void TearDown() {
        DestroyVisBuffer(&buffer);
    }
};


TEST_F(visBufferFunctions, BufferIsEmptyOnCreation) {
    ASSERT_TRUE(visBufferIsEmpty(buffer));

}


TEST_F(visBufferFunctions, addNode2Buffer){
    visBufferPushBackResult(buffer, NULL);
    size_t length = visBufferLength(buffer);
    ASSERT_EQ(length, 1);
}

TEST_F(visBufferFunctions, addCoupleNodes2Buffer){
    visBufferPushBackResult(buffer, NULL);
    visBufferPushBackResult(buffer, NULL);
    size_t length = visBufferLength(buffer);
    ASSERT_EQ(length, 2);
}

TEST_F(visBufferFunctions, addManyNodes2Buffer){
    for(int i = 0; i < 100; i++){
        ASSERT_EQ(visBufferPushBackResult(buffer, NULL), 0);
    }
}


TEST_F(visBufferFunctions, addResults2Buffer){
    const int TOTAL = 100;

    for(int i = 0; i < TOTAL; i++) {
        visVisualResult *res = CreateVisVisualResult();
        ASSERT_EQ(visBufferPushBackResult(buffer, res), 0);
    }

    for(int i = 0; i < TOTAL; i++) {
        visVisualResult *res = visBufferPopResult(buffer);
        ASSERT_FALSE(isVisVisualResultReady(res));
    }
}