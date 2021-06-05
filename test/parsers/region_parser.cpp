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
    utils::BoolMatrix twenty_by_twenty(20, std::vector<bool>(20, true));
    REQUIRE(region_parser::parse("20x20") == Region(20, 20, twenty_by_twenty));
}

TEST_CASE("Regions defined by map are parsed") {
    REQUIRE(region_parser::parse("x") == Region(1, 1, {{1}}));
    REQUIRE(region_parser::parse("x\nx\nx") == Region(1, 3, {{1}, {1}, {1}}));
    REQUIRE(region_parser::parse(" xx\nxx") == Region(3, 2, {{0, 1, 1}, {1, 1, 0}}));
    REQUIRE(region_parser::parse(" xxx\nxx") == Region(4, 2, {{0, 1, 1, 1}, {1, 1, 0, 0}}));
    REQUIRE(region_parser::parse("xxxxx \n    x \nxxx x \nx   x \nxxxxx ") ==
            Region(5, 5,
                   {{1, 1, 1, 1, 1},
                    {0, 0, 0, 0, 1},
                    {1, 1, 1, 0, 1},
                    {1, 0, 0, 0, 1},
                    {1, 1, 1, 1, 1}}));
}

TEST_CASE("Regions defined by border are parsed") {
    REQUIRE(region_parser::parse("DRUL") == Region(1, 1, {{1}}));
    REQUIRE(region_parser::parse("DDDRUUUL") == Region(1, 3, {{1}, {1}, {1}}));
    REQUIRE(region_parser::parse("ULLDLDRRUR") == Region(3, 2, {{0, 1, 1}, {1, 1, 0}}));
    REQUIRE(region_parser::parse("LURURRRDLLDL") == Region(4, 2, {{0, 1, 1, 1}, {1, 1, 0, 0}}));
    REQUIRE(region_parser::parse("DDDRRRRRUUUUULLLLLDRRRRDDDLLLURRULLL") ==
            Region(5, 5,
                   {{1, 1, 1, 1, 1},
                    {0, 0, 0, 0, 1},
                    {1, 1, 1, 0, 1},
                    {1, 0, 0, 0, 1},
                    {1, 1, 1, 1, 1}}));
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

    // holes in shape definitions are allowed for now
    // REQUIRE_THROWS_AS(region_parser::parse("xxx\nx x\nxxx"), ParseError);
    REQUIRE_NOTHROW(region_parser::parse_raw("xxx\nx x\nxxx"));

    REQUIRE_THROWS_AS(region_parser::parse("xxxxx\nx   x\nx x x\nx   x\nxxxxx"), ParseError);
    REQUIRE_NOTHROW(region_parser::parse_raw("xxxxx\nx   x\nx x x\nx   x\nxxxxx"));
}

TEST_CASE("Incorrectly defined regions are not parsed") {
    using region_parser::parse;
    REQUIRE_THROWS_AS(parse(""), ParseError);
    REQUIRE_THROWS_AS(parse(" "), ParseError);
    REQUIRE_THROWS_AS(parse("\n"), ParseError);
    REQUIRE_THROWS_AS(parse("3"), ParseError);
    REQUIRE_THROWS_AS(parse("3l"), ParseError);
    REQUIRE_THROWS_AS(parse("1xx"), ParseError);
    REQUIRE_THROWS_AS(parse("0x5"), ParseError);
    REQUIRE_THROWS_AS(parse("5x0"), ParseError);
    REQUIRE_THROWS_AS(parse("x\n\nx"), ParseError);
    REQUIRE_THROWS_AS(parse("d"), ParseError);
    REQUIRE_THROWS_AS(parse("D"), ParseError);
    REQUIRE_THROWS_AS(parse("DDRUL"), ParseError);
    REQUIRE_THROWS_AS(parse("DRUUDL"), ParseError);
    REQUIRE_THROWS_AS(parse("DRDRULUL"), ParseError);
    REQUIRE_THROWS_AS(parse("LLURDDRL"), ParseError);
}
