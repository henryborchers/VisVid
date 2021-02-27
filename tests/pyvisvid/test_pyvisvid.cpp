 //
// Created by Borchers, Henry Samuel on 2/26/21.
//

#include <catch.hpp>
#include <pyvisvid/utils.h>

TEST_CASE("Test version information"){
    std::string version_info = get_version();
    REQUIRE(!version_info.empty());
}
