#include "parsers/region_parser.hpp"

#include <vector>

#include "catch2/catch.hpp"
#include "parse_error.hpp"
#include "problem/region.hpp"
#include "utils.hpp"

TEST_CASE("Regions defined by name are parsed") {
    REQUIRE(region_parser::parse("1") == Region(1, 1, {{1}}));
    REQUIRE(region_parser::parse("3L") == Region(2, 2, {{1, 0}, {1, 1}}));
    REQUIRE(region_parser::parse("5U") == Region(3, 2, {{1, 0, 1}, {1, 1, 1}}));
}

TEST_CASE("Regions defined by dimensions are parsed") {
    REQUIRE(region_parser::parse("1x1") == Region(1, 1, {{1}}));
    utils::BoolMatrix two_by_four(4, std::vector<bool>(2, true));
    REQUIRE(region_parser::parse("2x4") == Region(2, 4, two_by_four));
    utils::BoolMatrix ten_by_one{std::vector<bool>(10, true)};
    REQUIRE(region_parser::parse("10x1") == Region(10, 1, ten_by_one));
}

TEST_CASE("Regions defined by map are parsed") {
    REQUIRE(region_parser::parse("x") == Region(1, 1, {{1}}));
    REQUIRE(region_parser::parse("x\nx\nx") == Region(1, 3, {{1}, {1}, {1}}));
    REQUIRE(region_parser::parse(" xx\nxx") == Region(3, 2, {{0, 1, 1}, {1, 1, 0}}));
    REQUIRE(region_parser::parse(" xxx\nxx") == Region(4, 2, {{0, 1, 1, 1}, {1, 1, 0, 0}}));
}

TEST_CASE("Region extra-whitespace trimming is performed") {
    REQUIRE(region_parser::parse("  xx\n xx") == Region(3, 2, {{0, 1, 1}, {1, 1, 0}}));
    REQUIRE(region_parser::parse_raw("  xx\n xx") == Region(4, 2, {{0, 0, 1, 1}, {0, 1, 1, 0}}));

    REQUIRE(region_parser::parse("  \n x \n ") == Region(1, 1, {{1}}));
    REQUIRE(region_parser::parse_raw("  \n x \n ") ==
            Region(3, 3, {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}));

    REQUIRE(region_parser::parse(" \n \n x \n \n ") == Region(1, 1, {{1}}));
    REQUIRE(region_parser::parse_raw(" \n \n x \n \n ") ==
            Region(3, 5, {{0, 0, 0}, {0, 0, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0, 0}}));
}

TEST_CASE("Continuity and holes in regions are handled") {
    REQUIRE_THROWS_AS(region_parser::parse("x\n x"), ParseError);
    REQUIRE_NOTHROW(region_parser::parse_raw("x\n x"));

    REQUIRE_THROWS_AS(region_parser::parse("xx x\nx  x\n xxx"), ParseError);
    REQUIRE_NOTHROW(region_parser::parse_raw("xx x\nx  x\n xxx"));

    REQUIRE_THROWS_AS(region_parser::parse("xxx\nx x\nxxx"), ParseError);
    REQUIRE_NOTHROW(region_parser::parse_raw("xxx\nx x\nxxx"));

    REQUIRE_THROWS_AS(region_parser::parse("xxxxx\nx   x\nx x x\nx   x\nxxxxx"), ParseError);
    REQUIRE_NOTHROW(region_parser::parse_raw("xxxxx\nx   x\nx x x\nx   x\nxxxxx"));
}

TEST_CASE("Incorrectly defined regions are not parsed") {
    REQUIRE_THROWS_AS(region_parser::parse(""), ParseError);
    REQUIRE_THROWS_AS(region_parser::parse(" "), ParseError);
    REQUIRE_THROWS_AS(region_parser::parse("\n"), ParseError);
    REQUIRE_THROWS_AS(region_parser::parse("3"), ParseError);
    REQUIRE_THROWS_AS(region_parser::parse("3l"), ParseError);
    REQUIRE_THROWS_AS(region_parser::parse("y"), ParseError);
    REQUIRE_THROWS_AS(region_parser::parse("1xx"), ParseError);
    REQUIRE_THROWS_AS(region_parser::parse("5x0"), ParseError);
    REQUIRE_THROWS_AS(region_parser::parse("x\n\nx"), ParseError);
}
