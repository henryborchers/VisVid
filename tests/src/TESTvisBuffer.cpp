//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <gtest/gtest.h>
extern "C"{
#include "visvid.h"
}


TEST(visBufferSetup, visBufferCreate){
    visBuffer *buffer = NULL;
    buffer = VisBuffer_Create2(0, 0);
    ASSERT_FALSE(buffer == NULL);
}
TEST(visBufferSetup, visBufferCreateAndDestroy){
    visBuffer *buffer = VisBuffer_Create2(0, 0);
    VisBuffer_Destroy(&buffer);
    ASSERT_TRUE(buffer == NULL);
}

class visBufferFunctions : public ::testing::Test {

protected:
    visBuffer *buffer;
    virtual void SetUp() {
        buffer = VisBuffer_Create2(10, 0);
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

TEST_F(visBufferFunctions, visBufferFunctions_pop_and_shift_Test){
    PixelValue              data_set1[] = {1,2,3,4,5,6,7,8,9,10};
    PixelValue              data_set2[] = {10,9,8,7,6,5,4,3,2,1};
    visVisualResult         first_res;
    visVisualResult         second_res;
    visVisualResult         third_res;
    visVisualResult         *current = NULL;

//    Create 3 results
    VisVisualResult_Init(&first_res);
    VisVisualResult_SetSize(&first_res, 10);
    VisVisualResult_SetData(&first_res, data_set1, 10);

    VisVisualResult_Init(&second_res);
    VisVisualResult_SetSize(&second_res, 10);
    VisVisualResult_SetData(&second_res, data_set2, 10);

    VisVisualResult_Init(&third_res);
    VisVisualResult_SetSize(&third_res, 10);
    VisVisualResult_SetData(&third_res, data_set2, 10);

//    Add those 3 results to a buffer
    visBuffer_PushBackResult(buffer, &first_res);
    visBuffer_PushBackResult(buffer, &second_res);
    visBuffer_PushBackResult(buffer, &third_res);

//    The buffer size should now be 3
    ASSERT_EQ(visBuffer_getLength(buffer), 3);

//    The buffer position for each node in the buffer should be perfectly aligned
    for(int i = 0; i < visBuffer_getLength(buffer); i++){
        visBufferNode *node =_BufferNode_get(buffer, i);
        ASSERT_EQ(_nodePosition(node), i);

    }

    current = visBuffer_PopShiftResult(buffer);
    for(int i =0 ; i < first_res.size; i++){
        PixelValue actual;
        PixelValue expected = data_set1[i];
        ASSERT_EQ(VisVisualResult_GetValue(&actual, current, i), 0);
        ASSERT_EQ(actual, expected) << "Expected: " << (int)expected << ", got " << (int)actual << ".";
    }

//    After popping the buffer should be only 2
    ASSERT_EQ(visBuffer_getLength(buffer), 2);

    // assert that the order is correct
    for(size_t i = 0; i < visBuffer_getLength(buffer); i++) {
        visBufferNode *node = NULL;
        node = _BufferNode_get(buffer, i);
        ASSERT_TRUE(node);
        int node_pos = _nodePosition(node);
        ASSERT_EQ(node_pos, i);
    }

    // pop and check again
    current = visBuffer_PopShiftResult(buffer);
//    The buffer size should now be 1 now
    ASSERT_EQ(visBuffer_getLength(buffer), 1);

    for(int i =0 ; i < second_res.size; i++){
        PixelValue actual;
        PixelValue expected = second_res.data[i];
        ASSERT_EQ(VisVisualResult_GetValue(&actual, current, i), 0);
        ASSERT_EQ(actual, expected) << "Expected: " << (int)expected << ", got " << (int)actual << ".";
    }



    current = visBuffer_PopShiftResult(buffer);
    ASSERT_EQ(visBuffer_getLength(buffer), 0);

    for(int i =0 ; i < third_res.size; i++){
        PixelValue actual;
        PixelValue expected = third_res.data[i];
        ASSERT_EQ(VisVisualResult_GetValue(&actual, current, i), 0);
        ASSERT_EQ(actual, expected) << "Expected: " << (int)expected << ", got " << (int)actual << ".";
    }

    current = visBuffer_PopShiftResult(buffer);
    ASSERT_TRUE(NULL == current);

}