//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid.h"
}


TEST_CASE("VisBuffer_Create2 --> not null", "[visbuffer]") {
    visBuffer *buffer = nullptr;
    buffer = VisBuffer_Create2(0, 0);
    REQUIRE(buffer != nullptr);

    VisBuffer_Destroy(&buffer);
    CHECK(buffer == nullptr);
}

TEST_CASE("VisBuffer_Destroy --> null", "[visbuffer]") {
    visBuffer *buffer = nullptr;
    buffer = VisBuffer_Create2(0, 0);
    CHECK(buffer != nullptr);

    VisBuffer_Destroy(&buffer);
    REQUIRE(buffer == nullptr);
}

TEST_CASE("visBufferFunctions", "[visbuffer]") {
    visBuffer *buffer = nullptr;
    buffer = VisBuffer_Create2(10, 0);
    CHECK(buffer != nullptr);

    SECTION("visBuffer is empty on creation") {
        REQUIRE(visBuffer_isEmpty(buffer) != 0);
    }

    SECTION("add Node to Buffer") {
        visBuffer_PushBackResult(buffer, nullptr);
        REQUIRE(visBuffer_getLength(buffer) == 1);
    }

    SECTION("Add Couple Nodes to Buffer") {
        visBuffer_PushBackResult(buffer, nullptr);
        visBuffer_PushBackResult(buffer, nullptr);
        REQUIRE(visBuffer_getLength(buffer) == 2);
    }

    SECTION("Add many Nodes to Buffer") {
        for (int i = 0; i < 100; i++) {
            CHECK(visBuffer_PushBackResult(buffer, nullptr) == 0);
        }
        REQUIRE(visBuffer_getLength(buffer) == 100);
    }

    SECTION("Add Results to Buffer") {
        const int TOTAL = 100;

        for (int i = 0; i < TOTAL; i++) {
            visVisualResult *res = VisVisualResult_Create();
            CHECK(visBuffer_PushBackResult(buffer, res) == 0);
        }

        for (int i = 0; i < TOTAL; i++) {
            visVisualResult *res = visBuffer_PopResult(buffer);
            REQUIRE(!VisVisualResult_IsReady(res));
            VisVisualResult_Destroy(&res);
        }
    }

    SECTION("get node position by position") {
        visBuffer_PushBackResult(buffer, nullptr);
        visBuffer_PushBackResult(buffer, nullptr);
        visBufferNode *node = nullptr;

        SECTION("check node at index 1 is the correct node") {
            node = _BufferNode_get(buffer, 1);
            REQUIRE(_nodePosition(node) == 1);

        }

//        TODO : Delete buffer after VisBufferNode_Destroy is fixed. Currently it doesn't return a null
//        VisBufferNode_Destroy(&node);
//        CHECK()
        SECTION("check node at index 1 is the correct node") {
            node = _BufferNode_get(buffer, 0);
            REQUIRE(_nodePosition(node) == 0);
//            VisBufferNode_Destroy(&node);
        }
    }

    SECTION("pop and shift 3 results") {
        PixelValue first_data_set[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        PixelValue second_data_set[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
        PixelValue third_data_set[10] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
        visVisualResult first_res;
        visVisualResult second_res;
        visVisualResult third_res;



//    Create 3 results
        VisVisualResult_Init(&first_res);
        VisVisualResult_SetSize(&first_res, 10);
        VisVisualResult_SetData(&first_res, first_data_set, 10);
        visBuffer_PushBackResult(buffer, &first_res);

        VisVisualResult_Init(&second_res);
        VisVisualResult_SetSize(&second_res, 10);
        VisVisualResult_SetData(&second_res, second_data_set, 10);
        visBuffer_PushBackResult(buffer, &second_res);

        VisVisualResult_Init(&third_res);
        VisVisualResult_SetSize(&third_res, 10);
        VisVisualResult_SetData(&third_res, third_data_set, 10);
        visBuffer_PushBackResult(buffer, &third_res);

        visBuffer_ShiftLeft(buffer);

        SECTION("First results") {
            visVisualResult *current = nullptr;
            current = visBuffer_PopResult(buffer);
            for (int i = 0; i < second_res.size; i++) {
                PixelValue actual = 0;
                PixelValue expected = second_res.data[i];
                CHECK(VisVisualResult_GetValue(&actual, current, i) == 0);
                INFO("Expected: " << (int) expected << ", got " << (int) actual << ".");
                REQUIRE(actual == expected);
            }


        }

        SECTION("Second result") {
            visVisualResult *current = nullptr;
            visBuffer_PopResult(buffer);
            current = visBuffer_PopResult(buffer);
            for (int i = 0; i < third_res.size; i++) {
                PixelValue actual = 0;
                PixelValue expected = third_res.data[i];
                CHECK(VisVisualResult_GetValue(&actual, current, i) == 0);
                INFO("Expected: " << (int) expected << ", got " << (int) actual << ".");
                REQUIRE(actual == expected);
            }
            VisVisualResult_Destroy(&current);

        }
        SECTION("Third result") {
            visVisualResult *current = nullptr;

            current = visBuffer_PopResult(buffer);
            VisVisualResult_Destroy(&current);

            current = visBuffer_PopResult(buffer);
            VisVisualResult_Destroy(&current);

            current = visBuffer_PopResult(buffer);
            for (int i = 0; i < first_res.size; i++) {
                PixelValue actual = 0;
                PixelValue expected = first_res.data[i];
                CHECK(VisVisualResult_GetValue(&actual, current, i) == 0);
                INFO("Expected: " << (int) expected << ", got " << (int) actual << ".");
                REQUIRE(actual == expected);
            }
        }
        SECTION("Ends in null") {
            visVisualResult *current = nullptr;

            current = visBuffer_PopResult(buffer);
            VisVisualResult_Destroy(&current);

            current = visBuffer_PopResult(buffer);
            VisVisualResult_Destroy(&current);

            current = visBuffer_PopResult(buffer);
            VisVisualResult_Destroy(&current);

            current = visBuffer_PopShiftResult(buffer);
            REQUIRE(nullptr == current);

        }
    }
    VisBuffer_Destroy(&buffer);
    CHECK(buffer == nullptr);
}

TEST_CASE("visBufferLimited", "[visbuffer]") {
    visBuffer *buffer = nullptr;
    buffer = VisBuffer_Create2(10, 5);
    CHECK(buffer != nullptr);

    SECTION("init_size") {
        REQUIRE(visBuffer_getLength(buffer) == 5);
    }

    SECTION("push data") {
        PixelValue first[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        PixelValue second[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
        PixelValue third[10] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
        PixelValue fourth[10] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
        PixelValue fifth[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
        PixelValue six[10] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6};

        visVisualResult res;
        VisVisualResult_Init(&res);
        VisVisualResult_SetSize(&res, 10);

        SECTION("First") {
            VisVisualResult_SetData(&res, first, 10);
            CHECK(visBuffer_PushBackResult(buffer, &res) == 0);
            CHECK(visBuffer_getLength(buffer) == 5);

        }

        SECTION("Second") {
            VisVisualResult_SetData(&res, second, 10);
            CHECK(visBuffer_PushBackResult(buffer, &res) == 0);
            CHECK(visBuffer_getLength(buffer) == 5);
        }

        SECTION("third") {
            VisVisualResult_SetData(&res, third, 10);
            CHECK(visBuffer_PushBackResult(buffer, &res) == 0);
            CHECK(visBuffer_getLength(buffer) == 5);

        }

        SECTION("fourth") {
            VisVisualResult_SetData(&res, fourth, 10);
            CHECK(visBuffer_PushBackResult(buffer, &res) == 0);
            CHECK(visBuffer_getLength(buffer) == 5);

        }

        SECTION("fifth") {
            VisVisualResult_SetData(&res, fifth, 10);
            CHECK(visBuffer_PushBackResult(buffer, &res) == 0);
            CHECK(visBuffer_getLength(buffer) == 5);

        }

        SECTION("six") {
            VisVisualResult_SetData(&res, six, 10);
            CHECK(visBuffer_PushBackResult(buffer, &res) == 0);
            CHECK(visBuffer_getLength(buffer) == 5);

        }
    }
    VisBuffer_Destroy(&buffer);
    CHECK(buffer == nullptr);
}