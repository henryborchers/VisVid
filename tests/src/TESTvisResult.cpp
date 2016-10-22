//
// Created by henry on 10/11/2016.
//
#include <gtest/gtest.h>


extern "C"{
#include "visResult.h"
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
    GetVisVisualResultReadySize(&size, result);
    ASSERT_EQ(size, 640);
}

TEST_F(visVisualResultFunctions, zeroOnInit){
    PixelValue value = 100;
    SetVisVisualResultReadySize(result, 10);
    GetVisVisualResultValue(&value, result, 5);
    ASSERT_EQ(value, 0);
}

TEST_F(visVisualResultFunctions, visVisualResultFunctions_getBadVaue_Test){
    PixelValue value = 100;
    SetVisVisualResultReadySize(result, 10);
    ASSERT_EQ(GetVisVisualResultValue(&value, result, 100), EFAULT);

}

TEST_F(visVisualResultFunctions, setresultdata) {
    PixelValue value = 100;
    PixelValue foo[] = {0,1,2,3,4,5,6,7,8,9};
    PixelValue *bar = (PixelValue *)malloc(sizeof(PixelValue ) * 10);
    bar[5] = 9;

    SetVisVisualResultReadySize(result, 10);

    // Test stack
    ASSERT_FALSE(isVisVisualResultReady(result));
    SetVisVisualResultData(result, foo, 10);
    GetVisVisualResultValue(&value, result, 5);
    ASSERT_EQ(value, 5);
    GetVisVisualResultValue(&value, result, 0);
    ASSERT_TRUE(isVisVisualResultReady(result));
    ASSERT_EQ(value, 0);

    // test heap
    SetVisVisualResultData(result, bar, 10);
    GetVisVisualResultValue(&value, result, 5);
    ASSERT_EQ(value, 9);
    bar[5] = 2;
    free(bar);
    ASSERT_EQ(value, 9);

}
