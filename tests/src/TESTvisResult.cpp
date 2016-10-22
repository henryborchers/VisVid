//
// Created by henry on 10/11/2016.
//
#include <gtest/gtest.h>


extern "C"{
#include <stdlib.h>
#include "visResult.h"
#include "visBuffer.h"
}

TEST(visVisualResultSetup, visVisualResultSetup_Create_Test) {
    visVisualResult *result = NULL;
    result = CreateVisVisualResult();
    ASSERT_FALSE(result == NULL);

}


TEST(visVisualResultSetup, visVisualResultSetup_Delete_Test) {
    visVisualResult *result = CreateVisVisualResult();
    DestroyVisVisualResult(&result);
    ASSERT_TRUE(result == NULL);
}

class visVisualResultFunctions : public ::testing::Test {
protected:
    visVisualResult *result;

    virtual void SetUp() {
        result = CreateVisVisualResult();
    }
    virtual void TearDown() {
        DestroyVisVisualResult(&result);
    }

};

TEST_F(visVisualResultFunctions, isVisVisualResultReady) {

    ASSERT_FALSE(isVisVisualResultReady(result));
}

TEST_F(visVisualResultFunctions, resize) {
    int size = -1;
    SetVisVisualResultReadySize(result, 640);
    GetVisVisualResultReadySize(result, &size);
    ASSERT_EQ(size, 640);
}

TEST_F(visVisualResultFunctions, zeroOnInit){
    uint8_t value = 100;
    SetVisVisualResultReadySize(result, 10);
    GetVisVisualResultValue(result, &value, 5);
    ASSERT_EQ(value, 0);
}

TEST_F(visVisualResultFunctions, visVisualResultFunctions_getBadVaue_Test){
    uint8_t value = 100;
    SetVisVisualResultReadySize(result, 10);
    ASSERT_EQ(GetVisVisualResultValue(result, &value, 100), EFAULT);

}

TEST_F(visVisualResultFunctions, setresultdata) {
    uint8_t value = 100;
    uint8_t foo[] = {0,1,2,3,4,5,6,7,8,9};
    uint8_t *bar = (uint8_t*)malloc(sizeof(uint8_t) * 10);
    bar[5] = 9;

    SetVisVisualResultReadySize(result, 10);

    // Test stack
    ASSERT_FALSE(isVisVisualResultReady(result));
    SetVisVisualResultData(result, foo, 10);
    GetVisVisualResultValue(result, &value, 5);
    ASSERT_EQ(value, 5);
    GetVisVisualResultValue(result, &value, 0);
    ASSERT_TRUE(isVisVisualResultReady(result));
    ASSERT_EQ(value, 0);

    // test heap
    SetVisVisualResultData(result, bar, 10);
    GetVisVisualResultValue(result, &value, 5);
    ASSERT_EQ(value, 9);
    bar[5] = 2;
    free(bar);
    bar = NULL;
    ASSERT_EQ(value, 9);

}