#include "problem/region.hpp"

#include <string>
#include <vector>

#include "catch2/catch.hpp"

TEST_CASE("Regions defined by name are correctly parsed") {
    REQUIRE(Region::parse("1") == Region(1, 1, {{true}}));
    REQUIRE(Region::parse("3L") == Region(2, 2, {{true, false}, {true, true}}));
    REQUIRE(Region::parse("5U") == Region(3, 2, {{true, false, true}, {true, true, true}}));
}

TEST_CASE("Regions defined by diemensions are correctly parsed") {
    REQUIRE(Region::parse("1x1") == Region(1, 1, {{true}}));
    std::vector<std::vector<bool>> two_by_four(4, std::vector<bool>(2, true));
    REQUIRE(Region::parse("2x4") == Region(2, 4, two_by_four));
    std::vector<std::vector<bool>> ten_by_one{std::vector<bool>(10, true)};
    REQUIRE(Region::parse("10x1") == Region(10, 1, ten_by_one));
}

TEST_CASE("Regions defined by map are correctly parsed") {
    REQUIRE(Region::parse("x") == Region(1, 1, {{true}}));
    REQUIRE(Region::parse("x\nx\nx") == Region(1, 3, {{true}, {true}, {true}}));
    REQUIRE(Region::parse(" xx\nxx") == Region(3, 2, {{false, true, true}, {true, true, false}}));
}
