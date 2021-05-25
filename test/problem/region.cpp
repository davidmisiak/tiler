#include "problem/region.hpp"

#include <vector>

#include "catch2/catch.hpp"
#include "parsers/region_parser.hpp"

TEST_CASE("Region can be rotated and reflected") {
    const auto& rp = region_parser::parse;

    REQUIRE(Region::rotate(rp("x")) == rp("x"));
    REQUIRE(Region::rotate(rp("xx")) == rp("x\nx"));
    REQUIRE(Region::rotate(rp("xx\nx\nx")) == rp("x\nxxx"));

    REQUIRE(Region::reflect(rp("x")) == rp("x"));
    REQUIRE(Region::reflect(rp("xx")) == rp("xx"));
    REQUIRE(Region::reflect(rp("xx\nx\nx")) == rp("xx\n x\n x"));
}

TEST_CASE("Region size and top-left getters work") {
    Region a = region_parser::parse("x");
    REQUIRE(a.get_size() == 1);
    REQUIRE(a.get_top_left_x() == 0);
    REQUIRE(a.get_top_left_y() == 0);

    Region b = region_parser::parse("  x\nxxx");
    REQUIRE(b.get_size() == 4);
    REQUIRE(b.get_top_left_x() == 2);
    REQUIRE(b.get_top_left_y() == 0);

    Region c = region_parser::parse_raw(" \n  x\nxxx");
    REQUIRE(c.get_size() == 4);
    REQUIRE(c.get_top_left_x() == 2);
    REQUIRE(c.get_top_left_y() == 1);

    Region d = region_parser::parse_raw(" ");
    REQUIRE(d.get_size() == 0);
    REQUIRE(d.get_top_left_x() == -1);
    REQUIRE(d.get_top_left_y() == -1);
}

TEST_CASE("Subregions can be manipulated") {
    //    xxxx         xxx
    //   xxxxx  ->     xxx
    //  xxxxx       xxxxx
    Region a = region_parser::parse("  xxxx\n xxxxx\nxxxxx");
    Region b = region_parser::parse(" x\nxx");
    Region original = a;

    REQUIRE(a.has_subregion(1, 0, b));
    REQUIRE(a.has_subregion(3, 1, b));
    REQUIRE_FALSE(a.has_subregion(3, 2, b));
    REQUIRE_FALSE(a.has_subregion(-1, 1, b));
    REQUIRE_FALSE(a.has_subregion(0, 0, region_parser::parse("x")));

    a.remove_subregion(1, 0, b);
    REQUIRE_FALSE(a.has_subregion(1, 0, b));
    REQUIRE(a == region_parser::parse("   xxx\n   xxx\nxxxxx"));
    REQUIRE(a.get_size() == 11);
    REQUIRE(a.get_top_left_x() == 3);
    REQUIRE(a.get_top_left_y() == 0);

    a.add_subregion(1, 0, b);
    REQUIRE(a.has_subregion(1, 0, b));
    REQUIRE(a == original);
    REQUIRE(a.get_size() == 14);
    REQUIRE(a.get_top_left_x() == 2);
    REQUIRE(a.get_top_left_y() == 0);

    REQUIRE(a.has_subregion(0, 0, original));

    a.remove_subregion(0, 0, original);
    REQUIRE_FALSE(a.has_subregion(0, 0, original));
    REQUIRE(a == region_parser::parse_raw("      \n      \n      "));
    REQUIRE(a.get_size() == 0);
    REQUIRE(a.get_top_left_x() == -1);
    REQUIRE(a.get_top_left_y() == -1);
}
