//
// Created by henry on 4/2/18.
//

#include "catch.hpp"

extern "C" {
#include "visvid/utils.h"
#include "visvid/visvid.h"
}


TEST_CASE("VisYUVFrame_Create --> not null", "[VisYUVFrame]"){
    VisYUVFrame *frame = VisYUVFrame_Create();
    REQUIRE(frame != nullptr);

    VisYUVFrame_Destroy(&frame);
    CHECK(frame == nullptr);

}
