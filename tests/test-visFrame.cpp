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