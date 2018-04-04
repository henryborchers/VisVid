//
// Created by henry on 4/3/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid.h"
}
SCENARIO("Visualization of a SolidColor"){
    GIVEN("a frame containing a single color"){

        VisYUVFrame     *frame = nullptr;

        visBrush        brush;

        int             width = 640;
        int             height = 480;

        brush.Y = 40;
        brush.U = 60;
        brush.V = 70;

        frame = VisYUVFrame_Create();

        CHECK(frame != nullptr);
        CHECK(VisYUVFrame_SetSize(frame, width, height) == 0);

        visYUVFrame_Fill(frame, &brush);

        WHEN("Calculating the brightness of the frame") {

            visVisualResult     *result = nullptr;

            result = VisVisualResult_Create();

            CHECK(result != nullptr);
            CHECK(VisVisualResult_SetSize(result, width) == 0);


            int buffersize = -1;
            VisVisualResult_GetSize(&buffersize, result);
            CHECK(visVisResult_CaculateBrightestOverWidth(result, frame) == 0);

            THEN("Each value in the result is the same"){
                for (int i = 0; i < buffersize; i++) {
                    PixelValue value = 0;
                    VisVisualResult_GetValue(&value, result, 0);
                    CHECK(value == 40);
                }
            }
            VisVisualResult_Destroy(&result);
        }
        VisYUVFrame_Destroy(&frame);
        CHECK(frame == nullptr);
    }
}

SCENARIO("Visualization Ramping Luma values") {
    GIVEN("A frame that's 100 by 480 pixels"){
        VisYUVFrame         *frame = nullptr;
        visBrush            brush;
        visVisualResult     result;

        int width = 100;
        int height = 480;

        brush.U = 60;
        brush.V = 70;

        VisVisualResult_Init(&result);

        CHECK(VisVisualResult_SetSize(&result, width) == 0);

        frame = VisYUVFrame_Create();
        CHECK(frame != nullptr);

        VisYUVFrame_SetSize(frame, width, height);
        WHEN("a the frame is drawn so that the luma matches the x position"){
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    brush.Y = (PixelValue) x;
                    YUVPixel_Draw(frame, &brush, x, y);

                }
            }

            THEN("The Brightest value for each is equal to the x position"){

                int buffersize = -1;

                VisVisualResult_GetSize(&buffersize, &result);
                CHECK(visVisResult_CaculateBrightestOverWidth(&result, frame) == 0);

                for(PixelValue i = 0; i < buffersize; i++){
                    PixelValue value = 0;

                    VisVisualResult_GetValue(&value, &result, i);
                    CHECK(value == i);
                }
            }
        }

        VisVisualResult_Cleanup(&result);
        VisYUVFrame_Destroy(&frame);
        CHECK(frame == nullptr);
    }

}