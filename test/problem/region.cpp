#include "problem/region.hpp"

#include <set>
#include <vector>

#include "catch2/catch.hpp"
#include "parsers/region_parser.hpp"
#include "utils.hpp"

// Region parser is used here because of readability.
// It should be tested thoroughly in it's own tests.

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

    a.add_subregion(1, 0, b);
    REQUIRE(a.has_subregion(1, 0, b));
    REQUIRE(a.get_size() == 3);
    REQUIRE(a.get_top_left_x() == 2);
    REQUIRE(a.get_top_left_y() == 0);
}

TEST_CASE("Correct region cells are returned") {
    using Sp = std::set<std::pair<int, int>>;

    auto a = region_parser::parse("1").get_cells();
    REQUIRE(a.size() == 1);
    REQUIRE(Sp(a.begin(), a.end()) == Sp{{0, 0}});

    auto b = region_parser::parse("4S").get_cells();
    REQUIRE(b.size() == 4);
    REQUIRE(Sp(b.begin(), b.end()) == Sp{{1, 0}, {2, 0}, {0, 1}, {1, 1}});

    auto c = region_parser::parse(" xxx\n x x\nxxxx").get_cells();
    REQUIRE(c.size() == 9);
    REQUIRE(Sp(c.begin(), c.end()) ==
            Sp{{1, 0}, {2, 0}, {3, 0}, {1, 1}, {3, 1}, {0, 2}, {1, 2}, {2, 2}, {3, 2}});
}

TEST_CASE("Correct region edges are returned") {
    using Se = std::set<utils::Edge>;

    auto a = region_parser::parse("1").get_edges();
    REQUIRE(a.size() == 4);
    REQUIRE(Se(a.begin(), a.end()) == Se{{0, 0, 0, 1}, {0, 0, 1, 0}, {1, 0, 0, 1}, {0, 1, 1, 0}});

    auto b = region_parser::parse("4S").get_edges();
    REQUIRE(b.size() == 10);
    REQUIRE(Se(b.begin(), b.end()) == Se{{1, 0, 1, 0},
                                         {2, 0, 1, 0},
                                         {0, 1, 1, 0},
                                         {2, 1, 1, 0},
                                         {0, 2, 1, 0},
                                         {1, 2, 1, 0},
                                         {1, 0, 0, 1},
                                         {3, 0, 0, 1},
                                         {0, 1, 0, 1},
                                         {2, 1, 0, 1}});

    auto c = region_parser::parse(" xxx\n x x\nxxxx").get_edges();
    REQUIRE(c.size() == 18);
    REQUIRE(Se(c.begin(), c.end()) == Se{{1, 0, 1, 0},
                                         {2, 0, 1, 0},
                                         {3, 0, 1, 0},
                                         {2, 1, 1, 0},
                                         {0, 2, 1, 0},
                                         {2, 2, 1, 0},
                                         {0, 3, 1, 0},
                                         {1, 3, 1, 0},
                                         {2, 3, 1, 0},
                                         {3, 3, 1, 0},
                                         {0, 2, 0, 1},
                                         {1, 0, 0, 1},
                                         {1, 1, 0, 1},
                                         {2, 1, 0, 1},
                                         {3, 1, 0, 1},
                                         {4, 0, 0, 1},
                                         {4, 1, 0, 1},
                                         {4, 2, 0, 1}});
}
