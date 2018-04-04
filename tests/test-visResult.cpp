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

TEST_CASE("VisVisualResult_SetSize --> not null", "[visVisualResult]"){


    visVisualResult result;
    VisVisualResult_Init(&result);
    VisVisualResult_SetSize(&result, 10);
    VisVisualResult_Cleanup(&result);

}
TEST_CASE("VisVisualResult_Destroy --> null", "[visVisualResult]"){

    visVisualResult *result = nullptr;

    result = VisVisualResult_Create();
    CHECK(nullptr != result);

    VisVisualResult_Destroy(&result);
    REQUIRE(nullptr == result);

}

TEST_CASE("VisVisualResult_Init --> data not null", "[visVisualResult]"){

    visVisualResult result;

    VisVisualResult_Init(&result);
    CHECK(VisVisualResult_SetSize(&result, 10) == 0);
    REQUIRE(result.data != nullptr);
    VisVisualResult_Cleanup(&result);
    REQUIRE(nullptr == result.data);

}

SCENARIO("visVisualResult Functions are used"){
    GIVEN("A pointer to a result struct is created on the heap"){

        visVisualResult *result = nullptr;
        result = VisVisualResult_Create();
        WHEN("nothing has been done to it"){
            THEN("the pointer to the newly created result is no longer null"){
                REQUIRE(nullptr != result);
            } AND_THEN("the pointer to the newly created result is listed as not ready"){
                REQUIRE(!VisVisualResult_IsReady(result));
            }

        }

        WHEN("VisVisualResult is resized to 640"){

            int size = -1;

            VisVisualResult_SetSize(result, 640);
            VisVisualResult_GetSize(&size, result);
            THEN("The size of the result is resized to 640"){
                REQUIRE(size == 640);
            } AND_THEN("The 5th element in the result is 0"){

                PixelValue value = 100;
                VisVisualResult_GetValue(&value, result, 5);
                INFO("Expected 255, received " << (int)value);
                REQUIRE((int)value == 0);
            }

        }

        WHEN("the VisVisualResult is resized to 10 and data on the stack is set to the VisVisualResult struct"){

            VisVisualResult_SetSize(result, 10);
            PixelValue foo[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

            CHECK(!VisVisualResult_IsReady(result));
            CHECK(VisVisualResult_SetData(result, foo, 10) == 0);

            THEN("The value is stored the result"){
                PixelValue value = 100;

                CHECK(VisVisualResult_GetValue(&value, result, 0) == 0);
                CHECK((int)value == 0);

                CHECK(VisVisualResult_GetValue(&value, result, 1) == 0);
                CHECK((int)value == 1);

                CHECK(VisVisualResult_GetValue(&value, result, 5) == 0);
                CHECK((int)value == 5);

                CHECK(VisVisualResult_GetValue(&value, result, 9) == 0);
                CHECK((int)value == 9);

            }

        }

        WHEN("the VisVisualResult is resized to 10 and data on the heap is set to the VisVisualResult struct"){

            auto *heap_data = (PixelValue *) malloc(sizeof(PixelValue) * 10);
            heap_data[0] = 0;
            heap_data[1] = 1;
            heap_data[5] = 5;
            heap_data[9] = 9;
            VisVisualResult_SetSize(result, 10);
            VisVisualResult_SetData(result, heap_data, 10);

            THEN("The value is stored the result"){
                PixelValue value = 100;

                CHECK(VisVisualResult_GetValue(&value, result, 0) == 0);
                CHECK((int)value == 0);

                CHECK(VisVisualResult_GetValue(&value, result, 1) == 0);
                CHECK((int)value == 1);

                CHECK(VisVisualResult_GetValue(&value, result, 5) == 0);
                CHECK((int)value == 5);

                CHECK(VisVisualResult_GetValue(&value, result, 9) == 0);
                CHECK((int)value == 9);

            }
            free(heap_data);
        }

        VisVisualResult_Destroy(&result);
        CHECK(nullptr == result);
    }

}