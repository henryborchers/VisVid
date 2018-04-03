//
// Created by henry on 4/3/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid.h"
}

TEST_CASE("VisualizationSolidColor") {

    VisYUVFrame *frame = nullptr;
    visVisualResult *result = nullptr;
    int width = 640;
    int height = 480;
    visBrush brush;

    brush.Y = 40;
    brush.U = 60;
    brush.V = 70;

    result = VisVisualResult_Create();
    CHECK(result != nullptr);

    CHECK(VisVisualResult_SetSize(result, width) == 0);

    frame = VisYUVFrame_Create();
    CHECK(frame != nullptr);

    CHECK(VisYUVFrame_SetSize(frame, width, height) == 0);

    visYUVFrame_Fill(frame, &brush);

    SECTION("Brightest") {

        int buffersize = -1;

        VisVisualResult_GetSize(&buffersize, result);
        CHECK(visVisResult_CaculateBrightestOverWidth(result, frame) == 0);

        for (int i = 0; i < buffersize; i++) {
            PixelValue value = 0;
            VisVisualResult_GetValue(&value, result, 0);
            CHECK(value == 40);
        }
    }

    VisYUVFrame_Destroy(&frame);
    CHECK(frame == nullptr);

}

TEST_CASE("VisualizationRampLuma") {
    VisYUVFrame *frame      = nullptr;
    visVisualResult *result = nullptr;
    int width               = 100;
    int height              = 480;

    visBrush brush;

    brush.U = 60;
    brush.V = 70;

    result = VisVisualResult_Create();
    CHECK(result != nullptr);

    CHECK(VisVisualResult_SetSize(result, width) == 0);

    frame = VisYUVFrame_Create();
    CHECK(frame != nullptr);

    VisYUVFrame_SetSize(frame, width, height);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            brush.Y = (PixelValue) x;
            YUVPixel_Draw(frame, &brush, x, y);

        }
    }

    SECTION("Brightest"){

        int buffersize = -1;

        VisVisualResult_GetSize(&buffersize, result);
        CHECK(visVisResult_CaculateBrightestOverWidth(result, frame) == 0);

        for(PixelValue i = 0; i < buffersize; i++){
            PixelValue value = 0;

            VisVisualResult_GetValue(&value, result, i);
            CHECK(value == i);
        }
    }

    VisYUVFrame_Destroy(&frame);
    CHECK(frame == nullptr);
}