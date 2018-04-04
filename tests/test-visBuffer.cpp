//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid.h"
}


TEST_CASE("VisBuffer_Create2 --> not null", "[visbuffer]") {
    visBuffer *buffer = nullptr;
    buffer = VisBuffer_Create2(10, 10);
    REQUIRE(buffer != nullptr);

    VisBuffer_Destroy(&buffer);
    CHECK(buffer == nullptr);
}

TEST_CASE("VisBuffer_Destroy --> null", "[visbuffer]") {
    visBuffer *buffer = nullptr;
    buffer = VisBuffer_Create2(10, 2);
    CHECK(buffer != nullptr);

    VisBuffer_Destroy(&buffer);
    REQUIRE(buffer == nullptr);
}

SCENARIO("visBuffer Functions are used"){
    GIVEN("A pointer to a 10 pixel wide buffer that is empty"){
        visBuffer *buffer = nullptr;
        buffer = VisBuffer_Create2(10, 0);

        WHEN("Nothing has been done to it"){
            THEN("the buffer is empty"){
                REQUIRE(visBuffer_isEmpty(buffer) != 0);
            }
        }

        WHEN("a node is added to the buffer"){
            visBuffer_PushBackResult(buffer, nullptr);

            THEN("the buffer size is 1"){
                REQUIRE(visBuffer_getLength(buffer) == 1);
            }
        }

        WHEN("a 2 nodes are added to the buffer"){
            visBuffer_PushBackResult(buffer, nullptr);
            visBuffer_PushBackResult(buffer, nullptr);

            THEN("the buffer size is 2"){
                REQUIRE(visBuffer_getLength(buffer) == 2);
            }
        }

        WHEN("100 nodes are added to the buffer"){
            for (int i = 0; i < 100; i++) {
                CHECK(visBuffer_PushBackResult(buffer, nullptr) == 0);
            }

            THEN("the buffer size is 100"){
                REQUIRE(visBuffer_getLength(buffer) == 100);
            }
        }

        WHEN("100 results are added to the buffer"){
            const int TOTAL = 100;

            for (int i = 0; i < TOTAL; i++) {
                visVisualResult res;
                VisVisualResult_Init(&res);

                CHECK(visBuffer_PushBackResult(buffer, &res) == 0);
            }

            THEN("the buffer size is 100"){

                REQUIRE(visBuffer_getLength(buffer) == 100);

                AND_THEN("and every result in the buffer is not ready"){
                    while(visBuffer_isEmpty(buffer)){
                        visVisualResult *res = nullptr;
                        res = visBuffer_PopResult(buffer);
                        CHECK(!VisVisualResult_IsReady(res));
                        VisVisualResult_Destroy(&res);
                    }
                }
            }
        }

        WHEN("the result has 3 data results"){
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

            AND_WHEN("The buffer is shifted to the left"){
                visBuffer_ShiftLeft(buffer);

                THEN("data in the first results in the buffer is the same data as the original second result"){

                    PixelValue currentSlice[10];
                    visBuffer_getResult(currentSlice, buffer,0);

                    for (int i = 0; i < second_res.size; i++) {
                        PixelValue expected = second_res.data[i];
                        PixelValue actual = currentSlice[i];

                        INFO("Expected: " << (int) expected << ", got " << (int) actual << ".");
                        REQUIRE(actual == expected);
                    }

                }

                THEN("data in the second results in the buffer is the same data as the original third result"){

                    PixelValue secondResult[10];
                    visBuffer_getResult(secondResult, buffer,1);

                    for (int i = 0; i < third_res.size; i++) {
                        PixelValue expected = third_res.data[i];
                        PixelValue actual = secondResult[i];

                        INFO("Expected: " << (int) expected << ", got " << (int) actual << ".");
                        REQUIRE(actual == expected);
                    }

                }

                THEN("data in the third results in the buffer is the same data as the original first result") {
                    PixelValue thirdSlice[10];
                    visBuffer_getResult(thirdSlice, buffer,2);

                    for (int i = 0; i < first_res.size; i++) {
                        PixelValue expected = first_res.data[i];
                        PixelValue actual = thirdSlice[i];

                        INFO("Expected: " << (int) expected << ", got " << (int) actual << ".");
                        REQUIRE(actual == expected);
                    }

                }
            }

            VisVisualResult_Cleanup(&first_res);
            VisVisualResult_Cleanup(&second_res);
            VisVisualResult_Cleanup(&third_res);
        }

        VisBuffer_Destroy(&buffer);
        CHECK(buffer == nullptr);
    }
}

TEST_CASE("visBufferFunctions", "[visbuffer]") {
    visBuffer *buffer = nullptr;
    buffer = VisBuffer_Create2(10, 0);
    CHECK(buffer != nullptr);

    SECTION("get node position by position") {
        visBuffer_PushBackResult(buffer, nullptr);
        visBuffer_PushBackResult(buffer, nullptr);
        visBufferNode *node = nullptr;

        SECTION("check node at index 1 is the correct node") {
            node = _BufferNode_get(buffer, 1);
            REQUIRE(_nodePosition(node) == 1);

        }

        SECTION("check node at index 1 is the correct node") {
            node = _BufferNode_get(buffer, 0);
            REQUIRE(_nodePosition(node) == 0);
        }
    }
    VisBuffer_Destroy(&buffer);
    CHECK(buffer == nullptr);
}

SCENARIO("Fixed Buffer size"){
    GIVEN("a buffer for 5 results of 10 units wide is created"){

        visBuffer *buffer = nullptr;
        buffer = VisBuffer_Create2(10, 5);
        CHECK(buffer != nullptr);
        CHECK(visBuffer_getLength(buffer) == 5);


        WHEN("6 arrays of pixel values contains values corresponding to their name are added to the buffer") {

            PixelValue first[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
            PixelValue second[10] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
            PixelValue third[10] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
            PixelValue fourth[10] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
            PixelValue fifth[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
            PixelValue six[10] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6};

            visVisualResult *res = nullptr;
            res = VisVisualResult_Create();
//        VisVisualResult_Init(res);
            VisVisualResult_SetSize(res, 10);

            CHECK(VisVisualResult_SetData(res, first, 10) == 0);
            CHECK(visBuffer_PushBackResult(buffer, res) == 0);

            CHECK(VisVisualResult_SetData(res, second, 10) == 0);
            CHECK(visBuffer_PushBackResult(buffer, res) == 0);

            VisVisualResult_SetData(res, third, 10);
            CHECK(visBuffer_PushBackResult(buffer, res) == 0);

            VisVisualResult_SetData(res, fourth, 10);
            CHECK(visBuffer_PushBackResult(buffer, res) == 0);

            VisVisualResult_SetData(res, fifth, 10);
            CHECK(visBuffer_PushBackResult(buffer, res) == 0);

            VisVisualResult_SetData(res, six, 10);
            CHECK(visBuffer_PushBackResult(buffer, res) == 0);

            THEN("The buffer length should remain 5"){
                REQUIRE(visBuffer_getLength(buffer) == 5);
            }

            VisVisualResult_Destroy(&res);
        }

        VisBuffer_Destroy(&buffer);
        CHECK(buffer == nullptr);

    }
}