//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid.h"
#include "visFrame.h"
}

TEST_CASE("visPixelYUV_Create --> not null", "[PixelYUV]"){
    PixelYUV *pixel = nullptr;
    pixel = PixelYUV_Create();
    REQUIRE(pixel != nullptr);

    PixelYUV_Destroy(&pixel);
    CHECK(pixel == nullptr);

}

TEST_CASE("PixelYUV_Destroy --> null", "[PixelYUV]"){
    PixelYUV *pixel = nullptr;
    pixel = PixelYUV_Create();
    CHECK(pixel != nullptr);

    PixelYUV_Destroy(&pixel);
    REQUIRE(pixel == nullptr);

}
SCENARIO("visPixelYUV can be get and set"){
    GIVEN("visPixelYUV is on the heap"){

        PixelYUV *pixel = nullptr;
        pixel = PixelYUV_Create();
        CHECK(pixel != nullptr);

        WHEN("PixelYUV is set"){

            PixelYUV_setValue(pixel, 4, 100, 40);

            THEN("The value is correctly assigned"){

                PixelValue y = 0;
                PixelValue u = 0;
                PixelValue v = 0;

                PixelYUV_getValue(pixel, &y, &u, &v);

                REQUIRE(y == 4);
                REQUIRE(u == 100);
                REQUIRE(v == 40);

            }
        }

        PixelYUV_Destroy(&pixel);
        CHECK(pixel == nullptr);
    }

}

TEST_CASE("VisYUVFrame_Create --> not null", "[VisYUVFrame]"){
    VisYUVFrame *frame = nullptr;
    frame = VisYUVFrame_Create();
    REQUIRE(frame != nullptr);

    VisYUVFrame_Destroy(&frame);
    CHECK(frame == nullptr);

}
SCENARIO("visFrameYUV Functions"){
    GIVEN("visFrameYUV is the heap"){

        VisYUVFrame *frame = nullptr;

        frame = VisYUVFrame_Create();
        CHECK(frame != nullptr);

        WHEN("nothing has been done to it"){
            THEN("it has no size"){
                int height = 0;
                int width = 0;

                VisYUVFrame_GetSize(frame, &width, &height);

                REQUIRE(height == -1);
                REQUIRE(width == -1);
            }
        }

        WHEN("the frame size is set to 640 by 480"){

            CHECK(VisYUVFrame_SetSize(frame, 640, 480) == 0);

            THEN("The size reads 640 by 480"){
                int height = 0;
                int width = 0;
                VisYUVFrame_GetSize(frame, &width, &height);
                REQUIRE(height == 480);
                REQUIRE(width == 640);
            }


        }

        WHEN("The frame is filled by brush"){

            int height = 100;
            int width = 200;

            VisYUVFrame_SetSize(frame, width, height);

            visBrush brush;

            brush.Y = 20;
            brush.U = 30;
            brush.V = 40;

            THEN("The frame any pixel found in the frame is painted that colored"){

                PixelYUV pix;

                CHECK(visYUVFrame_Fill(frame, &brush) == 0);
                CHECK(VisYUVFrame_getPixelYUV(&pix, frame, 50, 50) == 0);

                PixelValue y = 0;
                PixelValue u = 0;
                PixelValue v = 0;
                PixelYUV_getValue(&pix, &y, &u, &v);

                REQUIRE(y == brush.Y);
                REQUIRE(u == brush.U);
                REQUIRE(v == brush.V);

            }

        }
        WHEN("a brush is used to draw on every pixel the frame") {

            int height  = 20;
            int width   = 100;

            visBrush brush;

            brush.Y     = 20;
            brush.U     = 30;
            brush.V     = 40;

            PixelYUV pix;


            VisYUVFrame_SetSize(frame, width, height);
            for(PixelValue x = 0; x < width; x++){
                brush.Y = x;
                YUVPixel_Draw(frame, &brush, x, 0);
            }

            THEN("All pixels in the frame have the color of the brush"){

                for(PixelValue x = 0; x < width; x++){

                    PixelValue y = x;
                    PixelValue u = 0;
                    PixelValue v = 0;

                    CHECK(VisYUVFrame_getPixelYUV(&pix, frame, x, 0) == 0);
                    PixelYUV_getValue(&pix, &y, &u, &v);

                    CHECK(y == x);
                    CHECK(u == brush.U);
                    CHECK(v == brush.V);
                }
            }
        }

        VisYUVFrame_Destroy(&frame);
        CHECK(frame == nullptr);

    }
}
