#include "utils.hpp"

#include "catch2/catch.hpp"

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
