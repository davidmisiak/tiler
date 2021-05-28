#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "catch2/catch.hpp"
#include "parsers/problem_parser.hpp"
#include "problem/problem.hpp"
#include "problem/tile.hpp"
#include "solution/solution.hpp"
#include "solvers/sat_solver.hpp"
#include "solvers/simple_solver.hpp"

// Problem parser is used here because of readability.
// It should be tested thoroughly in it's own tests.

namespace {

bool check_solution(Problem problem, Solution solution, bool expected) {
    if (!expected) {
        return solution.size() == 0;
    }
    for (auto [x, y, region] : solution) {
        if (!problem.board_.has_subregion(x, y, region)) return false;
        problem.board_.remove_subregion(x, y, region);
        bool found = false;
        for (Tile& tile : problem.tiles_) {
            if (std::find(tile.begin(), tile.end(), region) == tile.end()) continue;
            if (tile.get_count() == 0) continue;
            tile.add_count(-1);
            found = true;
            break;
        }
        if (!found) return false;
    }
    return problem.board_.get_size() == 0;
}

}  // namespace

TEMPLATE_TEST_CASE("Solvers return correct solutions", "", SimpleSolver, SatSolver) {
    std::vector<std::tuple<std::vector<std::string>, bool, bool>> problems = {
            {{"1", "1:1"}, false, true},
            {{"1", "2:1"}, false, true},
            {{"1", "1"}, false, true},
            {{"3L", "3:1"}, false, true},
            {{"3L", "1:3L"}, false, true},
            {{"3L", "1:2", "1:1"}, false, true},
            {{"5V", "1:5V"}, false, true},
            {{"4x2", "8:1"}, false, true},
            {{"4x2", "1:3L", "1:4T", "1:1"}, false, true},
            {{"4x2", "1:2", "1:4I", "1:2"}, false, true},
            {{"3x4", "2:RRRDDDLUULDLUU"}, false, true},
            {{"3x3", "1:xxx\nx x\nxxx", "1:1"}, false, true},
            {{"DDDDDRRRRUURRUUULLLLLL", "2:3I", "2:4O", "2:4J", "1:4Z"}, false, true},
            {{"DDRRDDDDDRDRURULLUURRRDDRDRUULUULUUUULLDDRDDLLUULLUL", "1:4T", "1:4J", "1:5G",
              "1:4I", "1:5V", "1:4O"},
             false,
             true},

            {{"4L", "1:4J"}, true, true},
            {{"DDRDRUUURDRUUUULLLDRRDLLDL", "1:5P", "2:4L"}, true, true},

            {{"2", "1:1"}, false, false},
            {{"1", "2"}, false, false},
            {{"3L", "1:2"}, false, false},
            {{"3L", "2"}, false, false},
            {{"5V", "1:3L", "2"}, false, false},
            {{"4x2", "7:1"}, false, false},
            {{"3x4", "1:4O", "1:4T", "1:4J"}, false, false},
            {{"3x5", "5V"}, false, false},
            {{"DDRDRRRRRRULLLLULULL", "1:4I", "1:3I", "1:4O"}, false, false},
            {{"4L", "1:4J"}, false, false},
            {{"5P", "1:xxx\nx x\nxxx", "1:1"}, false, false},
            {{"5P", "1:xxx\nx x\nxxx", "1:2"}, false, false},
            {{"DDDRDRRRUUULULLL", "2"}, false, false},

            {{"3x4", "4L"}, true, false},
    };

    for (auto [problem_str, reflection, result] : problems) {
        Problem problem = problem_parser::parse(problem_str, reflection);
        Solution solution = TestType(problem).solve();
        REQUIRE(check_solution(problem, solution, result));
    }
}
