#include "problem/tile.hpp"

#include "catch2/catch.hpp"
#include "parsers/region_parser.hpp"
#include "parsers/tile_parser.hpp"

// Tile parser is used here because of readability.
// It should be tested thoroughly in it's own tests.

TEST_CASE("Tile rotations and reflections are correct") {
    const auto& rp = region_parser::parse;

    Tile a = tile_parser::parse("x", false);
    REQUIRE(a.end() - a.begin() == 1);
    REQUIRE(std::count(a.begin(), a.end(), rp("x")) == 1);

    Tile b = tile_parser::parse("x", true);
    REQUIRE(b.end() - b.begin() == 1);
    REQUIRE(std::count(b.begin(), b.end(), rp("x")) == 1);

    Tile c = tile_parser::parse("xx", false);
    REQUIRE(c.end() - c.begin() == 2);
    REQUIRE(std::count(c.begin(), c.end(), rp("xx")) == 1);
    REQUIRE(std::count(c.begin(), c.end(), rp("x\nx")) == 1);

    Tile d = tile_parser::parse("xx", true);
    REQUIRE(d.end() - d.begin() == 2);
    REQUIRE(std::count(d.begin(), d.end(), rp("xx")) == 1);
    REQUIRE(std::count(d.begin(), d.end(), rp("x\nx")) == 1);

    Tile e = tile_parser::parse("xx\nx", false);
    REQUIRE(e.end() - e.begin() == 4);
    REQUIRE(std::count(e.begin(), e.end(), rp("xx\nx ")) == 1);
    REQUIRE(std::count(e.begin(), e.end(), rp("x \nxx")) == 1);
    REQUIRE(std::count(e.begin(), e.end(), rp(" x\nxx")) == 1);
    REQUIRE(std::count(e.begin(), e.end(), rp("xx\n x")) == 1);

    Tile f = tile_parser::parse("xx\nx", true);
    REQUIRE(f.end() - f.begin() == 4);
    REQUIRE(std::count(f.begin(), f.end(), rp("xx\nx ")) == 1);
    REQUIRE(std::count(f.begin(), f.end(), rp("x \nxx")) == 1);
    REQUIRE(std::count(f.begin(), f.end(), rp(" x\nxx")) == 1);
    REQUIRE(std::count(f.begin(), f.end(), rp("xx\n x")) == 1);

    Tile g = tile_parser::parse("xx\nx\nx", false);
    REQUIRE(g.end() - g.begin() == 4);
    REQUIRE(std::count(g.begin(), g.end(), rp("xx\nx \nx ")) == 1);
    REQUIRE(std::count(g.begin(), g.end(), rp("x  \nxxx")) == 1);
    REQUIRE(std::count(g.begin(), g.end(), rp(" x\n x\nxx")) == 1);
    REQUIRE(std::count(g.begin(), g.end(), rp("xxx\n  x")) == 1);

    Tile h = tile_parser::parse("xx\nx\nx", true);
    REQUIRE(h.end() - h.begin() == 8);
    REQUIRE(std::count(h.begin(), h.end(), rp("xx\nx \nx ")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), rp("x  \nxxx")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), rp(" x\n x\nxx")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), rp("xxx\n  x")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), rp("xx\n x\n x")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), rp("  x\nxxx")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), rp("x \nx \nxx")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), rp("xxx\nx  ")) == 1);
}
