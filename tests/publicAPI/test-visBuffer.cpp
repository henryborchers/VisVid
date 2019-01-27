//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid/visvid.h"
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

