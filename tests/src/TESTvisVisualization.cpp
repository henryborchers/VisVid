//
// Created by Borchers, Henry Samuel on 10/22/16.
//

#include <gtest/gtest.h>

extern "C"{
#include "visView.h"
#include "visFrame.h"
#include "visVisualization.h"
}

class VisualizationSolidColor : public ::testing::Test {

protected:
    VisYUVFrame     *frame;
    visVisualResult *result;



    virtual void SetUp() {
        int width = 640;
        int height = 480;
        result = VisVisualResult_Create();
        VisVisualResult_SetSize(result, width);

        frame = VisYUVFrame_Create();
        VisYUVFrame_setSize(frame, width, height);
        visBrush brush;
        brush.Y = 40;
        brush.U = 60;
        brush.V = 70;
        visYUVFrame_Fill(frame, &brush);
    }
    virtual void TearDown() {
        VisYUVFrame_Destroy(&frame);
    }
};

TEST_F(VisualizationSolidColor, Brightest) {
    int buffersize = -1;
    VisVisualResult_GetSize(&buffersize, result);
    ASSERT_EQ(visVisResult_CaculateBrightestOverWidth(result, frame), 0);
    for(int i = 0; i < buffersize; i++){
        PixelValue value = 0;
        VisVisualResult_GetValue(&value, result, 0);
        ASSERT_EQ(value, 40);
    }
}


class VisualizationRampLuma : public ::testing::Test {

protected:
    VisYUVFrame     *frame;
    visVisualResult *result;



    virtual void SetUp() {
        int width = 100;
        int height = 480;
        result = VisVisualResult_Create();
        VisVisualResult_SetSize(result, width);

        frame = VisYUVFrame_Create();
        VisYUVFrame_setSize(frame, width, height);
        visBrush brush;
        brush.U = 60;
        brush.V = 70;
        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++){
                brush.Y = (PixelValue)x;
                YUVPixel_Draw(frame, &brush, x, y);

            }
        }

    }
    virtual void TearDown() {
        VisYUVFrame_Destroy(&frame);
    }
};

TEST_F(VisualizationRampLuma, Brightest) {
    int buffersize = -1;
    VisVisualResult_GetSize(&buffersize, result);
    ASSERT_EQ(visVisResult_CaculateBrightestOverWidth(result, frame), 0);
    for(PixelValue i = 0; i < buffersize; i++){
        PixelValue value = 0;
        VisVisualResult_GetValue(&value, result, i);
        ASSERT_EQ(value, i);
    }
}


class VisualizationProcess : public ::testing::Test {

protected:
    visProcessContext ctx;
    virtual void SetUp() {

        ctx.processCb = visVisResult_CaculateBrightestOverWidth;

    }
};

TEST_F(VisualizationProcess, BadArgs) {
    ASSERT_EQ(visVisProcess(NULL, NULL, &ctx), EFAULT);
}
