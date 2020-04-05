//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid/visvid.h"
//#include "visView.h"
}

TEST_CASE("VisView_Create --> not null", "[visView]"){


    visView *pvid = VisView_Create(640, 480);
    REQUIRE(nullptr != pvid);

    REQUIRE(pvid->width == 640);
    REQUIRE(pvid->height == 480);

    VisView_Destroy(&pvid);
    CHECK(nullptr == pvid);

}

TEST_CASE("VisView_Destroy --> null", "[visView]"){

    visView *pvid = VisView_Create(640, 480);
    CHECK(nullptr != pvid);


    VisView_Destroy(&pvid);
    REQUIRE(nullptr == pvid);

}
//

TEST_CASE("visView_Update4 fails on empty buffer", "[visView]"){

    visView *pvid = VisView_Create(640, 480);
    visBuffer *buffer = VisBuffer_Create2(10, 2);
    CHECK(visView_Update4(pvid, buffer) < 0);
    VisBuffer_Destroy(&buffer);
    VisView_Destroy(&pvid);

}
//

