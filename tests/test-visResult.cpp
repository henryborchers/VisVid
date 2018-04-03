//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid.h"
}

TEST_CASE("VisVisualResult_Create --> not null", "[visVisualResult]"){


    visVisualResult *result = nullptr;

    result = VisVisualResult_Create();
    REQUIRE(nullptr != result);

    VisVisualResult_Destroy(&result);
    CHECK(nullptr == result);

}
TEST_CASE("VisVisualResult_Destroy --> null", "[visVisualResult]"){


    visVisualResult *result = nullptr;

    result = VisVisualResult_Create();
    CHECK(nullptr != result);

    VisVisualResult_Destroy(&result);
    REQUIRE(nullptr == result);

}

TEST_CASE("visVisualResultFunctions"){

    visVisualResult *result = nullptr;

    result = VisVisualResult_Create();
    CHECK(nullptr != result);

    SECTION("VisVisualResult is not ready on creation"){
        REQUIRE(!VisVisualResult_IsReady(result));
    }

    SECTION("VisVisualResult can be resized"){

        int size = -1;

        VisVisualResult_SetSize(result, 640);
        VisVisualResult_GetSize(&size, result);

        REQUIRE(size == 640);
    }

    SECTION("VisVisualResult has a value of zero On Initizaition"){

        PixelValue value = 100;

        VisVisualResult_SetSize(result, 10);
        VisVisualResult_GetValue(&value, result, 5);

        REQUIRE(value == 0);
    }

    SECTION("Set Result data of VisVisualResult"){
        PixelValue value = 100;
        PixelValue foo[] = {0,1,2,3,4,5,6,7,8,9};
        auto *bar = (PixelValue *)malloc(sizeof(PixelValue) * 10);
        bar[5] = 9;

        VisVisualResult_SetSize(result, 10);

        // Test stack
        SECTION("Stack") {

            CHECK(!VisVisualResult_IsReady(result));

            VisVisualResult_SetData(result, foo, 10);
            VisVisualResult_GetValue(&value, result, 5);
            CHECK(value == 5);

            VisVisualResult_GetValue(&value, result, 0);
            CHECK(VisVisualResult_IsReady(result));
            REQUIRE(value == 0);

        }

        // test heap
        SECTION("Heap"){

            VisVisualResult_SetData(result, bar, 10);
            VisVisualResult_GetValue(&value, result, 5);
            CHECK(value == 9);

            bar[5] = 2;
            free(bar);
            REQUIRE(value == 9);

        }
    }

    VisVisualResult_Destroy(&result);
    CHECK(nullptr == result);
}