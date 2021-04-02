#include "problem/region.hpp"

#include <string>
#include <vector>

#include "catch2/catch.hpp"
#include "parse_error.hpp"
#include "utils.hpp"

TEST_CASE("Regions defined by name are parsed") {
    REQUIRE(Region::parse("1") == Region(1, 1, {{1}}));
    REQUIRE(Region::parse("3L") == Region(2, 2, {{1, 0}, {1, 1}}));
    REQUIRE(Region::parse("5U") == Region(3, 2, {{1, 0, 1}, {1, 1, 1}}));
}

TEST_CASE("Regions defined by dimensions are parsed") {
    REQUIRE(Region::parse("1x1") == Region(1, 1, {{1}}));
    utils::BoolMatrix two_by_four(4, std::vector<bool>(2, true));
    REQUIRE(Region::parse("2x4") == Region(2, 4, two_by_four));
    utils::BoolMatrix ten_by_one{std::vector<bool>(10, true)};
    REQUIRE(Region::parse("10x1") == Region(10, 1, ten_by_one));
}

TEST_CASE("Regions defined by map are parsed") {
    REQUIRE(Region::parse("x") == Region(1, 1, {{1}}));
    REQUIRE(Region::parse("x\nx\nx") == Region(1, 3, {{1}, {1}, {1}}));
    REQUIRE(Region::parse(" xx\nxx") == Region(3, 2, {{0, 1, 1}, {1, 1, 0}}));
}
