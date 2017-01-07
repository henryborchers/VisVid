//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <gtest/gtest.h>
extern "C"{
#include "visvid.h"
}


TEST(visBufferSetup, visBufferCreate){
    visBuffer *buffer = NULL;
    buffer = VisBuffer_Create(0);
    ASSERT_FALSE(buffer == NULL);
}
TEST(visBufferSetup, visBufferCreateAndDestroy){
    visBuffer *buffer = VisBuffer_Create(0);
    VisBuffer_Destroy(&buffer);
    ASSERT_TRUE(buffer == NULL);
}

class visBufferFunctions : public ::testing::Test {

protected:
    visBuffer *buffer;
    virtual void SetUp() {
        buffer = VisBuffer_Create(10);
    }

    virtual void TearDown() {
        VisBuffer_Destroy(&buffer);
    }
};


TEST_F(visBufferFunctions, BufferIsEmptyOnCreation) {
    ASSERT_TRUE(visBuffer_isEmpty(buffer));

}

TEST_F(visBufferFunctions, addNode2Buffer){
    visBuffer_PushBackResult(buffer, NULL);
    size_t length = visBuffer_getLength(buffer);
    ASSERT_EQ(length, 1);
}

TEST_F(visBufferFunctions, addCoupleNodes2Buffer){
    visBuffer_PushBackResult(buffer, NULL);
    visBuffer_PushBackResult(buffer, NULL);
    size_t length = visBuffer_getLength(buffer);
    ASSERT_EQ(length, 2);
}

TEST_F(visBufferFunctions, addManyNodes2Buffer){
    for(int i = 0; i < 100; i++){
        ASSERT_EQ(visBuffer_PushBackResult(buffer, NULL), 0);
    }
}


TEST_F(visBufferFunctions, addResults2Buffer){
    const int TOTAL = 100;

    for(int i = 0; i < TOTAL; i++) {
        visVisualResult *res = VisVisualResult_Create();
        ASSERT_EQ(visBuffer_PushBackResult(buffer, res), 0);
    }

    for(int i = 0; i < TOTAL; i++) {
        visVisualResult *res = visBuffer_PopResult(buffer);
        ASSERT_FALSE(VisVisualResult_IsReady(res));
    }
}

TEST_F(visBufferFunctions, get_node_position){
    visBuffer_PushBackResult(buffer, NULL);
    visBuffer_PushBackResult(buffer, NULL);
    visBufferNode *node = NULL;

    node = _BufferNode_get(buffer, 1);
    ASSERT_EQ(_nodePosition(node), 1);

    node = _BufferNode_get(buffer, 0);
    ASSERT_EQ(_nodePosition(node), 0);
}