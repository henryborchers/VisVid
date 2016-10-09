//
// Created by Borchers, Henry Samuel on 10/8/16.
//

#include <gtest/gtest.h>
extern "C"{
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
    GetVisVisualResultReadyValue(result, &value, 5);
    ASSERT_EQ(value, 0);
}

TEST_F(visVisualResultFunctions, visVisualResultFunctions_getBadVaue_Test){
    uint8_t value = 100;
    SetVisVisualResultReadySize(result, 10);
    ASSERT_EQ(GetVisVisualResultReadyValue(result, &value, 100), EFAULT);

}