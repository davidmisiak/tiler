#include "parsers/problem_parser.hpp"

#include <string>

#include "catch2/catch.hpp"
#include "parse_error.hpp"
#include "parsers/region_parser.hpp"
#include "problem/problem.hpp"

TEST_CASE("Problems are parsed") {
    std::string a =
            "x\n"
            "\n"
            "x";
    Problem ap = problem_parser::parse(a, false);
    REQUIRE(ap.board_ == region_parser::parse("x"));
    REQUIRE(ap.tiles_.size() == 1);

    std::string b =
            "2x2\n"
            "\n"
            "\n"
            "1:x\n"
            "\n"
            "3L\n"
            "\n";
    Problem bp = problem_parser::parse(b, false);
    REQUIRE(bp.board_ == region_parser::parse("2x2"));
    REQUIRE(bp.tiles_.size() == 2);

    std::string c =
            "xxx\n"
            " xx  \n"
            "\n"
            "1: \n"
            " xx\n";
    Problem cp = problem_parser::parse(c, false);
    REQUIRE(cp.board_ == region_parser::parse("xxx\n xx"));
    REQUIRE(cp.tiles_.size() == 1);
}

TEST_CASE("Incorrectly defined problems are not parsed") {
    std::string a = "x";
    REQUIRE_THROWS_AS(problem_parser::parse(a, false), ParseError);

    std::string b =
            "x\n"
            "\n";
    REQUIRE_THROWS_AS(problem_parser::parse(b, false), ParseError);

    std::string c =
            "x\n"
            "\n"
            "1:\n"
            "\n"
            "x\n";
    REQUIRE_THROWS_AS(problem_parser::parse(c, false), ParseError);

    std::string d =
            "x\n"
            " x\n"
            "\n"
            "x\n";
    REQUIRE_THROWS_AS(problem_parser::parse(d, false), ParseError);

    std::string e =
            "xxx\n"
            " xx  \n"
            "\n"
            " 1:\n"
            " xx\n";
    REQUIRE_THROWS_AS(problem_parser::parse(e, false), ParseError);

    std::string f =
            "xxx\n"
            " xx\n"
            " \n"
            "1:\n"
            " xx\n";
    REQUIRE_THROWS_AS(problem_parser::parse(f, false), ParseError);
}
