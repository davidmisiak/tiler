#include "parsers/tile_parser.hpp"

#include "catch2/catch.hpp"
#include "parse_error.hpp"
#include "parsers/region_parser.hpp"
#include "problem/tile.hpp"

TEST_CASE("Unlimited tiles are parsed") {
    Tile a = tile_parser::parse("1", false);
    REQUIRE(*a.begin() == region_parser::parse("1"));
    REQUIRE(a.get_count() == -1);

    Tile b = tile_parser::parse("3L", false);
    REQUIRE(*b.begin() == region_parser::parse("3L"));
    REQUIRE(b.get_count() == -1);

    Tile c = tile_parser::parse("2x4", false);
    REQUIRE(*c.begin() == region_parser::parse("2x4"));
    REQUIRE(c.get_count() == -1);

    Tile d = tile_parser::parse("x", false);
    REQUIRE(*d.begin() == region_parser::parse("x"));
    REQUIRE(d.get_count() == -1);

    Tile e = tile_parser::parse(" xx\nxx", false);
    REQUIRE(*e.begin() == region_parser::parse(" xx\nxx"));
    REQUIRE(e.get_count() == -1);

    Tile f = tile_parser::parse("  xx\n xx", false);
    REQUIRE(*f.begin() == region_parser::parse(" xx\nxx"));
    REQUIRE(f.get_count() == -1);
}

TEST_CASE("Limited tiles are parsed") {
    Tile a = tile_parser::parse("1:1", false);
    REQUIRE(*a.begin() == region_parser::parse("1"));
    REQUIRE(a.get_count() == 1);

    Tile b = tile_parser::parse("3:3L", false);
    REQUIRE(*b.begin() == region_parser::parse("3L"));
    REQUIRE(b.get_count() == 3);

    Tile c = tile_parser::parse("4:2x4", false);
    REQUIRE(*c.begin() == region_parser::parse("2x4"));
    REQUIRE(c.get_count() == 4);

    Tile d = tile_parser::parse("1:x", false);
    REQUIRE(*d.begin() == region_parser::parse("x"));
    REQUIRE(d.get_count() == 1);

    Tile e = tile_parser::parse("2: xx\nxx", false);
    REQUIRE(*e.begin() == region_parser::parse(" xx\nxx"));
    REQUIRE(e.get_count() == 2);

    Tile f = tile_parser::parse("3:\n  xx\n xx", false);
    REQUIRE(*f.begin() == region_parser::parse(" xx\nxx"));
    REQUIRE(f.get_count() == 3);
}

TEST_CASE("Incorrectly defined tiles are not parsed") {
    REQUIRE_THROWS_AS(tile_parser::parse(" ", false), ParseError);
    REQUIRE_THROWS_AS(tile_parser::parse(":", false), ParseError);
    REQUIRE_THROWS_AS(tile_parser::parse("1:", false), ParseError);
    REQUIRE_THROWS_AS(tile_parser::parse(":x", false), ParseError);
    REQUIRE_THROWS_AS(tile_parser::parse("1::x", false), ParseError);
    REQUIRE_THROWS_AS(tile_parser::parse("1:x:x", false), ParseError);
    REQUIRE_THROWS_AS(tile_parser::parse("1\n:x", false), ParseError);
    REQUIRE_THROWS_AS(tile_parser::parse("0:x", false), ParseError);
}
