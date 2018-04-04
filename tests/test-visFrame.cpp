//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid.h"
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
TEST_CASE("visFrameYUV Functions"){
    VisYUVFrame *frame = nullptr;

    frame = VisYUVFrame_Create();
    CHECK(frame != nullptr);

    SECTION("visFrameYUV are empty on creation"){
        int height = 0;
        int width = 0;
        int64_t pos = 0;
        VisYUVFrame_GetSize(frame, &width, &height);
        VisYUVFrame_SetPos(frame, &pos);

        REQUIRE(height == -1);
        REQUIRE(width == -1);
        REQUIRE(pos == -1);
    }

    SECTION("Set frame size"){
        CHECK(VisYUVFrame_SetSize(frame, 640, 480) == 0);
        int height = 0;
        int width = 0;
        VisYUVFrame_GetSize(frame, &width, &height);
        REQUIRE(height == 480);
        REQUIRE(width == 640);
    }

    SECTION("Brush fill"){
        int height = 100;
        int width = 200;

        visBrush brush;
        brush.Y = 20;
        brush.U = 30;
        brush.V = 40;
        PixelYUV pix;

        VisYUVFrame_SetSize(frame, width, height);
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
    SECTION("Brush Draw") {
        int height = 20;
        int width = 100;
        visBrush brush;
        brush.Y = 20;
        brush.U = 30;
        brush.V = 40;
        PixelYUV pix;


        VisYUVFrame_SetSize(frame, width, height);
        for(PixelValue x = 0; x < width; x++){
            brush.Y = x;
            YUVPixel_Draw(frame, &brush, x, 0);
        }

        for(PixelValue x = 0; x < width; x++){
            CHECK(VisYUVFrame_getPixelYUV(&pix, frame, x, 0) == 0);
            PixelValue y = x;
            PixelValue u = 0;
            PixelValue v = 0;
            PixelYUV_getValue(&pix, &y, &u, &v);

            CHECK(y == x);
            CHECK(u == brush.U);
            CHECK(v == brush.V);
        }

    }

    VisYUVFrame_Destroy(&frame);
    CHECK(frame == nullptr);

}