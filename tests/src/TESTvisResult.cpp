//
// Created by henry on 10/11/2016.
//
#include <gtest/gtest.h>


extern "C"{
#include "include/visvid/visvid.h"
}

TEST(visVisualResultSetup, visVisualResultSetup_Create_Test) {
    visVisualResult *result = NULL;
    result = VisVisualResult_Create();
    ASSERT_FALSE(result == NULL);

}


TEST(visVisualResultSetup, visVisualResultSetup_Delete_Test) {
    visVisualResult *result = VisVisualResult_Create();
    VisVisualResult_Destroy(&result);
    ASSERT_TRUE(result == NULL);
}

class visVisualResultFunctions : public ::testing::Test {
protected:
    visVisualResult *result;

    virtual void SetUp() {
        result = VisVisualResult_Create();
    }
    virtual void TearDown() {
        VisVisualResult_Destroy(&result);
    }

};

TEST_F(visVisualResultFunctions, VisVisualResult_IsReady) {

    ASSERT_FALSE(VisVisualResult_IsReady(result));
}

TEST_F(visVisualResultFunctions, resize) {
    int size = -1;
    VisVisualResult_SetSize(result, 640);
    VisVisualResult_GetSize(&size, result);
    ASSERT_EQ(size, 640);
}

TEST_F(visVisualResultFunctions, zeroOnInit){
    PixelValue value = 100;
    VisVisualResult_SetSize(result, 10);
    VisVisualResult_GetValue(&value, result, 5);
    ASSERT_EQ(value, 0);
}
#ifdef BADTESTS
TEST_F(visVisualResultFunctions, visVisualResultFunctions_getBadVaue_Test){
    PixelValue value = 100;
    VisVisualResult_SetSize(result, 10);
    ASSERT_EQ(VisVisualResult_GetValue(&value, result, 100), EFAULT);

}
#endif
TEST_F(visVisualResultFunctions, setresultdata) {
    PixelValue value = 100;
    PixelValue foo[] = {0,1,2,3,4,5,6,7,8,9};
    PixelValue *bar = (PixelValue *)malloc(sizeof(PixelValue) * 10);
    bar[5] = 9;

    VisVisualResult_SetSize(result, 10);

    // Test stack
    ASSERT_FALSE(VisVisualResult_IsReady(result));
    VisVisualResult_SetData(result, foo, 10);
    VisVisualResult_GetValue(&value, result, 5);
    ASSERT_EQ(value, 5);
    VisVisualResult_GetValue(&value, result, 0);
    ASSERT_TRUE(VisVisualResult_IsReady(result));
    ASSERT_EQ(value, 0);

    // tests heap
    VisVisualResult_SetData(result, bar, 10);
    VisVisualResult_GetValue(&value, result, 5);
    ASSERT_EQ(value, 9);
    bar[5] = 2;
    free(bar);
    ASSERT_EQ(value, 9);

}
