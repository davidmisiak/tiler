#include "problem/problem.hpp"

#include <string>

#include "catch2/catch.hpp"
#include "parse_error.hpp"
#include "problem/board.hpp"

TEST_CASE("Problems are parsed") {
    std::string a =
            "x\n"
            "\n"
            "x";
    Problem ap = Problem::create(a, false);
    REQUIRE(ap.board_ == Board::parse("x"));
    REQUIRE(ap.tiles_.size() == 1);

    std::string b =
            "2x2\n"
            "\n"
            "\n"
            "1:x\n"
            "\n"
            "3L\n"
            "\n";
    Problem bp = Problem::create(b, false);
    REQUIRE(bp.board_ == Board::parse("2x2"));
    REQUIRE(bp.tiles_.size() == 2);

    std::string c =
            "xxx\n"
            " xx  \n"
            "\n"
            "1: \n"
            " xx\n";
    Problem cp = Problem::create(c, false);
    REQUIRE(cp.board_ == Board::parse("xxx\n xx"));
    REQUIRE(cp.tiles_.size() == 1);
}

TEST_CASE("Incorrectly defined problems are not parsed") {
    std::string a = "x";
    REQUIRE_THROWS_AS(Problem::create(a, false), ParseError);

    std::string b =
            "x\n"
            "\n";
    REQUIRE_THROWS_AS(Problem::create(b, false), ParseError);

    std::string c =
            "x\n"
            "\n"
            "1:\n"
            "\n"
            "x\n";
    REQUIRE_THROWS_AS(Problem::create(c, false), ParseError);

    std::string d =
            "x\n"
            " x\n"
            "\n"
            "x\n";
    REQUIRE_THROWS_AS(Problem::create(d, false), ParseError);

    std::string e =
            "xxx\n"
            " xx  \n"
            "\n"
            " 1:\n"
            " xx\n";
    REQUIRE_THROWS_AS(Problem::create(e, false), ParseError);

    std::string f =
            "xxx\n"
            " xx\n"
            " \n"
            "1:\n"
            " xx\n";
    REQUIRE_THROWS_AS(Problem::create(f, false), ParseError);
}
