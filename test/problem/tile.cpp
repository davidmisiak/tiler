#include "problem/tile.hpp"

#include "catch2/catch.hpp"
#include "parse_error.hpp"
#include "problem/region.hpp"
#include "utils.hpp"

TEST_CASE("Unlimited tiles are parsed") {
    Tile a = Tile::parse("1", false);
    REQUIRE(*a.begin() == Region::parse("1"));
    REQUIRE(a.get_count() == -1);

    Tile b = Tile::parse("3L", false);
    REQUIRE(*b.begin() == Region::parse("3L"));
    REQUIRE(b.get_count() == -1);

    Tile c = Tile::parse("2x4", false);
    REQUIRE(*c.begin() == Region::parse("2x4"));
    REQUIRE(c.get_count() == -1);

    Tile d = Tile::parse("x", false);
    REQUIRE(*d.begin() == Region::parse("x"));
    REQUIRE(d.get_count() == -1);

    Tile e = Tile::parse(" xx\nxx", false);
    REQUIRE(*e.begin() == Region::parse(" xx\nxx"));
    REQUIRE(e.get_count() == -1);

    Tile f = Tile::parse("  xx\n xx", false);
    REQUIRE(*f.begin() == Region::parse(" xx\nxx"));
    REQUIRE(f.get_count() == -1);
}

TEST_CASE("Limited tiles are parsed") {
    Tile a = Tile::parse("1:1", false);
    REQUIRE(*a.begin() == Region::parse("1"));
    REQUIRE(a.get_count() == 1);

    Tile b = Tile::parse("3:3L", false);
    REQUIRE(*b.begin() == Region::parse("3L"));
    REQUIRE(b.get_count() == 3);

    Tile c = Tile::parse("4:2x4", false);
    REQUIRE(*c.begin() == Region::parse("2x4"));
    REQUIRE(c.get_count() == 4);

    Tile d = Tile::parse("1:x", false);
    REQUIRE(*d.begin() == Region::parse("x"));
    REQUIRE(d.get_count() == 1);

    Tile e = Tile::parse("2: xx\nxx", false);
    REQUIRE(*e.begin() == Region::parse(" xx\nxx"));
    REQUIRE(e.get_count() == 2);

    Tile f = Tile::parse("3:\n  xx\n xx", false);
    REQUIRE(*f.begin() == Region::parse(" xx\nxx"));
    REQUIRE(f.get_count() == 3);
}

TEST_CASE("Incorrectly defined tiles are not parsed") {
    REQUIRE_THROWS_AS(Tile::parse(" ", false), ParseError);
    REQUIRE_THROWS_AS(Tile::parse(":", false), ParseError);
    REQUIRE_THROWS_AS(Tile::parse("1:", false), ParseError);
    REQUIRE_THROWS_AS(Tile::parse(":x", false), ParseError);
    REQUIRE_THROWS_AS(Tile::parse("1::x", false), ParseError);
    REQUIRE_THROWS_AS(Tile::parse("1:x:x", false), ParseError);
    REQUIRE_THROWS_AS(Tile::parse("1\n:x", false), ParseError);
    REQUIRE_THROWS_AS(Tile::parse("0:x", false), ParseError);
}

TEST_CASE("Tile rotations and reflections are correct") {
    Tile a = Tile::parse("x", false);
    REQUIRE(a.end() - a.begin() == 1);
    REQUIRE(std::count(a.begin(), a.end(), Region::parse("x")) == 1);

    Tile b = Tile::parse("x", true);
    REQUIRE(b.end() - b.begin() == 1);
    REQUIRE(std::count(b.begin(), b.end(), Region::parse("x")) == 1);

    Tile c = Tile::parse("xx", false);
    REQUIRE(c.end() - c.begin() == 2);
    REQUIRE(std::count(c.begin(), c.end(), Region::parse("xx")) == 1);
    REQUIRE(std::count(c.begin(), c.end(), Region::parse("x\nx")) == 1);

    Tile d = Tile::parse("xx", true);
    REQUIRE(d.end() - d.begin() == 2);
    REQUIRE(std::count(d.begin(), d.end(), Region::parse("xx")) == 1);
    REQUIRE(std::count(d.begin(), d.end(), Region::parse("x\nx")) == 1);

    Tile e = Tile::parse("xx\nx", false);
    REQUIRE(e.end() - e.begin() == 4);
    REQUIRE(std::count(e.begin(), e.end(), Region::parse("xx\nx ")) == 1);
    REQUIRE(std::count(e.begin(), e.end(), Region::parse("x \nxx")) == 1);
    REQUIRE(std::count(e.begin(), e.end(), Region::parse(" x\nxx")) == 1);
    REQUIRE(std::count(e.begin(), e.end(), Region::parse("xx\n x")) == 1);

    Tile f = Tile::parse("xx\nx", true);
    REQUIRE(f.end() - f.begin() == 4);
    REQUIRE(std::count(f.begin(), f.end(), Region::parse("xx\nx ")) == 1);
    REQUIRE(std::count(f.begin(), f.end(), Region::parse("x \nxx")) == 1);
    REQUIRE(std::count(f.begin(), f.end(), Region::parse(" x\nxx")) == 1);
    REQUIRE(std::count(f.begin(), f.end(), Region::parse("xx\n x")) == 1);

    Tile g = Tile::parse("xx\nx\nx", false);
    REQUIRE(g.end() - g.begin() == 4);
    REQUIRE(std::count(g.begin(), g.end(), Region::parse("xx\nx \nx ")) == 1);
    REQUIRE(std::count(g.begin(), g.end(), Region::parse("x  \nxxx")) == 1);
    REQUIRE(std::count(g.begin(), g.end(), Region::parse(" x\n x\nxx")) == 1);
    REQUIRE(std::count(g.begin(), g.end(), Region::parse("xxx\n  x")) == 1);

    Tile h = Tile::parse("xx\nx\nx", true);
    REQUIRE(h.end() - h.begin() == 8);
    REQUIRE(std::count(h.begin(), h.end(), Region::parse("xx\nx \nx ")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), Region::parse("x  \nxxx")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), Region::parse(" x\n x\nxx")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), Region::parse("xxx\n  x")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), Region::parse("xx\n x\n x")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), Region::parse("  x\nxxx")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), Region::parse("x \nx \nxx")) == 1);
    REQUIRE(std::count(h.begin(), h.end(), Region::parse("xxx\nx  ")) == 1);
}
