//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid/visvid.h"
#include "visView.h"
}

TEST_CASE("VisView_Create --> not null", "[visView]"){


    visView *pvid = nullptr;

    pvid = VisView_Create(640, 480);
    REQUIRE(nullptr != pvid);

    REQUIRE(pvid->width == 640);
    REQUIRE(pvid->height == 480);

    VisView_Destroy(&pvid);
    CHECK(nullptr == pvid);

}

TEST_CASE("VisView_Destroy --> null", "[visView]"){

    visView *pvid = nullptr;

    pvid = VisView_Create(640, 480);
    CHECK(nullptr != pvid);


    VisView_Destroy(&pvid);
    REQUIRE(nullptr == pvid);

}

TEST_CASE("visView Functions on an Empty Buffer"){

    visView *pvid = nullptr;

    pvid = VisView_Create(640, 480);
    CHECK(nullptr != pvid);

    SECTION("visView is empty on creation"){
        visBuffer *Buffer = VisBuffer_Create2(10, 0);
        CHECK(Buffer != NULL);
        int rc = visView_Update(pvid, Buffer);
        CHECK(rc == 0);


        for(int x = 0; x < pvid->width; x++){
            for(int y = 0; y < pvid->height; y++){
                CHECK(pvid->data[x + pvid->width * y] == 0);

            }
        }
        VisBuffer_Destroy(&Buffer);
        CHECK(Buffer == nullptr);
    }

    VisView_Destroy(&pvid);
    CHECK(nullptr == pvid);

}
SCENARIO("visView full buffer"){
    GIVEN("visView Functions on a full buffer of empty data"){
        visView *pvid = nullptr;
        visBuffer *buffer = nullptr;

        buffer= VisBuffer_Create2(10, 0);
        CHECK(buffer != nullptr);

        pvid = VisView_Create(2, 480);
        CHECK(pvid != nullptr);


        WHEN("The buffer has no data"){
            visBuffer_PushBackResult(buffer, nullptr);
            visBuffer_PushBackResult(buffer, nullptr);

            CHECK(visView_Update(pvid, buffer) == 0);

            THEN("All the data in the view is completely black"){
                for(int x = 0; x<pvid->width; x++){
                    for(int y = 0; y<pvid->height; y++){
                        CHECK(pvid->data[x + pvid->width * y] == 0);
                    }

                };
            }

            AND_WHEN("The view is converted into a RGB image"){

                visImageRGB imageRGB;


                CHECK(visImageRGB_Alloc(&imageRGB, pvid->width, pvid->height) == 0);
                CHECK(visViewRGB_GenerateRGBA(&imageRGB, pvid, visViewRGBA_value2BW) == 0);

                THEN("Resulting image is completely black"){

                    const int ALPHA_SHIFT = 0;
                    const int BLUE_SHIFT  = 1;
                    const int GREEN_SHIFT = 2;
                    const int RED_SHIFT   = 3;

                    for (int y = 0; y < imageRGB.height; ++y) {
                        for (int x = 0; x < imageRGB.width; ++x) {
                            int offset = (y * imageRGB.pitch) + (x * sizeof(uint8_t) * 4);
                            CHECK(imageRGB.plane[offset + ALPHA_SHIFT] == 0);
                            CHECK(imageRGB.plane[offset + RED_SHIFT] == 0);
                            CHECK(imageRGB.plane[offset + BLUE_SHIFT] == 0);
                            CHECK(imageRGB.plane[offset + GREEN_SHIFT] == 0);
                        }
                    }
                }

                visImageRGB_FreeData(&imageRGB);
            }

        }


        VisView_Destroy(&pvid);
        CHECK(pvid == nullptr);

        VisBuffer_Destroy(&buffer);
        CHECK(buffer == nullptr);

    }
}
