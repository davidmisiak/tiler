#include "utils.hpp"

#include "catch2/catch.hpp"

TEST_CASE("Flood-fill works") {
    REQUIRE(utils::flood_fill(0, 0, 1, 1, {{0}}, {}) == utils::BoolMatrix{{1}});
    REQUIRE(utils::flood_fill(0, 0, 1, 2, {{1}, {0}}, {}) == utils::BoolMatrix{{0}, {0}});
    REQUIRE(utils::flood_fill(0, 0, 3, 1, {{0, 0, 0}}, {}) == utils::BoolMatrix{{1, 1, 1}});
    REQUIRE(utils::flood_fill(0, 0, 3, 1, {{0, 1, 0}}, {}) == utils::BoolMatrix{{1, 1, 0}});
    REQUIRE(utils::flood_fill(0, 0, 3, 1, {{0, 0, 0}}, {{0, 0, 0, 1}}) ==
            utils::BoolMatrix{{1, 1, 1}});
    REQUIRE(utils::flood_fill(0, 0, 3, 1, {{0, 0, 0}}, {{1, 0, 0, 1}}) ==
            utils::BoolMatrix{{1, 0, 0}});
    REQUIRE(utils::flood_fill(1, 0, 3, 1, {{0, 0, 0}}, {{1, 0, 0, 1}, {2, 0, 0, 1}}) ==
            utils::BoolMatrix{{0, 1, 0}});

    REQUIRE(utils::flood_fill(1, 0, 3, 2, {{0, 1, 0}, {1, 1, 1}},
                              {{1, 0, 0, 1}, {0, 1, 1, 0}, {2, 1, 0, 1}}) ==
            utils::BoolMatrix{{0, 0, 0}, {0, 0, 1}});

    REQUIRE(utils::flood_fill(1, 1, 3, 2, {{0, 1, 1}, {1, 1, 1}},
                              {{1, 0, 0, 1}, {0, 1, 1, 0}, {2, 1, 0, 1}}) ==
            utils::BoolMatrix{{0, 0, 0}, {0, 0, 0}});

    REQUIRE(utils::flood_fill(1, 2, 3, 4, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                              {{1, 1, 1, 0},
                               {2, 1, 1, 0},
                               {2, 2, 1, 0},
                               {1, 3, 1, 0},
                               {1, 1, 0, 1},
                               {1, 2, 0, 1},
                               {2, 2, 0, 1},
                               {3, 1, 0, 1}}) ==
            utils::BoolMatrix{{0, 0, 0}, {0, 1, 1}, {0, 1, 0}, {0, 0, 0}});

    REQUIRE(utils::flood_fill(0, 3, 3, 4, {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}},
                              {{2, 2, 0, 1}, {2, 3, 0, 1}}) ==
            utils::BoolMatrix{{0, 0, 0}, {1, 1, 1}, {1, 1, 0}, {1, 1, 0}});
}

TEST_CASE("Margins are trimmed") {
    using Result = std::tuple<int, int, utils::BoolMatrix>;
    REQUIRE(utils::remove_margins(0, 0, {}) == Result{0, 0, {}});
    REQUIRE(utils::remove_margins(0, 1, {{}}) == Result{0, 0, {}});
    REQUIRE(utils::remove_margins(1, 0, {}) == Result{0, 0, {}});
    REQUIRE(utils::remove_margins(1, 1, {{0}}) == Result{0, 0, {}});
    REQUIRE(utils::remove_margins(1, 1, {{1}}) == Result{1, 1, {{1}}});
    REQUIRE(utils::remove_margins(1, 3, {{0}, {0}, {0}}) == Result{0, 0, {}});
    REQUIRE(utils::remove_margins(2, 1, {{0, 1}}) == Result{1, 1, {{1}}});
    REQUIRE(utils::remove_margins(2, 2, {{0, 1}, {1, 0}}) == Result{2, 2, {{0, 1}, {1, 0}}});
    REQUIRE(utils::remove_margins(2, 2, {{0, 1}, {0, 0}}) == Result{1, 1, {{1}}});
    REQUIRE(utils::remove_margins(5, 5,
                                  {{0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0},
                                   {0, 1, 1, 0, 0},
                                   {0, 0, 1, 0, 0},
                                   {0, 0, 0, 0, 0}}) == Result{2, 2, {{1, 1}, {0, 1}}});
}

TEST_CASE("Continuity check works") {
    REQUIRE(utils::is_continuous(0, 0, {}));
    REQUIRE(utils::is_continuous(0, 1, {{}}));
    REQUIRE(utils::is_continuous(1, 0, {}));
    REQUIRE(utils::is_continuous(1, 1, {{0}}));
    REQUIRE(utils::is_continuous(1, 1, {{1}}));
    REQUIRE(utils::is_continuous(3, 1, {{1, 1, 0}}));
    REQUIRE_FALSE(utils::is_continuous(3, 1, {{1, 0, 1}}));
    REQUIRE(utils::is_continuous(2, 3, {{0, 1}, {1, 1}, {1, 0}}));
    REQUIRE(utils::is_continuous(3, 3, {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}));
    REQUIRE(utils::is_continuous(3, 3, {{1, 1, 0}, {1, 0, 1}, {1, 1, 1}}));
    REQUIRE_FALSE(utils::is_continuous(3, 3, {{1, 1, 0}, {1, 0, 1}, {0, 1, 1}}));
    REQUIRE_FALSE(utils::is_continuous(6, 5,
                                       {{1, 1, 1, 1, 1, 0},
                                        {1, 1, 0, 0, 1, 0},
                                        {1, 0, 1, 0, 1, 0},
                                        {1, 0, 0, 0, 1, 0},
                                        {1, 1, 1, 1, 1, 0}}));
}

TEST_CASE("Holes are checked") {
    REQUIRE_FALSE(utils::has_hole(0, 0, {}));
    REQUIRE_FALSE(utils::has_hole(0, 1, {{}}));
    REQUIRE_FALSE(utils::has_hole(1, 0, {}));
    REQUIRE_FALSE(utils::has_hole(1, 1, {{0}}));
    REQUIRE_FALSE(utils::has_hole(1, 1, {{1}}));
    REQUIRE_FALSE(utils::has_hole(3, 1, {{1, 1, 0}}));
    REQUIRE_FALSE(utils::has_hole(3, 3, {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}));
    REQUIRE_FALSE(utils::has_hole(3, 3, {{1, 1, 0}, {1, 1, 1}, {1, 1, 1}}));
    REQUIRE(utils::has_hole(3, 3, {{1, 1, 0}, {1, 0, 1}, {1, 1, 1}}));
    REQUIRE_FALSE(utils::has_hole(3, 3, {{1, 1, 1}, {0, 0, 0}, {1, 1, 1}}));
    REQUIRE(utils::has_hole(4, 4, {{1, 1, 1, 1}, {1, 1, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 1}}));
    REQUIRE(utils::has_hole(6, 5,
                            {{0, 0, 0, 0, 0, 0},
                             {0, 1, 1, 1, 0, 0},
                             {0, 1, 1, 0, 1, 0},
                             {0, 1, 1, 1, 1, 0},
                             {0, 0, 0, 0, 0, 0}}));
}
