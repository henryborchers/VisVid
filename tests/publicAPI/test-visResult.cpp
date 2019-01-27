//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid/visvid.h"
//    #include "visResult.h"
}


TEST_CASE("VisVisualResult_SetSize --> not null", "[visVisualResult]"){


    visVisualResult result;
    VisVisualResult_Init(&result);
    VisVisualResult_SetSize(&result, 10);
    VisVisualResult_Cleanup(&result);

}

TEST_CASE("VisVisualResult_Init --> data not null", "[visVisualResult]"){

    visVisualResult result;

    VisVisualResult_Init(&result);
    CHECK(VisVisualResult_SetSize(&result, 10) == 0);
    REQUIRE(result.data != nullptr);
    VisVisualResult_Cleanup(&result);
    REQUIRE(nullptr == result.data);

}

