#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "catch2/catch.hpp"
#include "errors/not_implemented_error.hpp"
#include "errors/time_limit_error.hpp"
#include "parsers/problem_parser.hpp"
#include "print.hpp"
#include "problem/problem.hpp"
#include "problem/tile.hpp"
#include "solution/solution.hpp"
#include "solvers/solver_factory.hpp"
#include "utils.hpp"

// Problem parser is used here because of readability.
// It should be tested thoroughly in it's own tests.

namespace {

bool check_solution(Problem problem, Solution solution, bool expected) {
    if (!expected) {
        return solution.size() == 0;
    }
    for (const auto& [x, y, region] : solution) {
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

void test_solving(Problem problem, std::string problem_ref, bool result, int max_seconds) {
    print::normal() << problem_ref << std::endl;
    for (const std::string& solver_name : solver_factory::get_solver_names()) {
        try {
            Solution solution =
                    solver_factory::create(solver_name, problem)->solve(false, max_seconds);
            std::vector<std::string> info_words = {solver_name, "should return",
                                                   (result ? "true" : "false"), "on", problem_ref};
            INFO(boost::algorithm::join(info_words, " "));
            REQUIRE(check_solution(problem, solution, result));
        } catch (const TimeLimitError& e) {
            print::normal() << "  timeout: " << solver_name << std::endl;
            // If you don't want this to ever happed, set `max_seconds` to 0 (disabled).
        } catch (const NotImplementedError& e) {
            print::normal() << "  not implemented: " << solver_name << std::endl;
        }
    }
}

}  // namespace

TEST_CASE("Solvers return correct solutions") {
    std::vector<std::tuple<std::vector<std::string>, bool, bool>> problems = {
            {{"1", "1:1"}, false, true},
            {{"1", "2:1"}, false, true},
            {{"1", "1"}, false, true},
            {{"3L", "3:1"}, false, true},
            {{"3L", "1:3L"}, false, true},
            {{"3L", "1:2", "1:1"}, false, true},
            {{"3L", "1:5N", "1:2", "1:1"}, false, true},
            {{"5V", "1:5V"}, false, true},
            {{"4x2", "8:1"}, false, true},
            {{"4x2", "1:3L", "1:4T", "1:1"}, false, true},
            {{"4x2", "1:2", "1:4I", "1:2"}, false, true},
            {{"3x3", "1:1", "5:2"}, false, true},
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
            {{"4L", "3L"}, false, false},

            {{"3x4", "4L"}, true, false},
            {{"5V", "3I", "1:1"}, true, false},
    };

    for (const auto& [problem_strs, reflection, result] : problems) {
        Problem problem = problem_parser::parse(problem_strs, reflection);
        std::string problem_ref = boost::algorithm::join(problem_strs, " ");
        test_solving(problem, problem_ref, result, 0);
    }
}

TEST_CASE("Solvers return correct solutions (benchmark problems)") {
    for (const std::string& filepath : utils::get_file_paths("problems")) {
        bool is_solvable = boost::algorithm::ends_with(filepath, "_s");
        bool is_unsolvable = boost::algorithm::ends_with(filepath, "_u");
        bool reflection = filepath.find('\'') != std::string::npos;
        if (is_solvable || is_unsolvable) {
            Problem problem = problem_parser::parse_from_file(filepath, reflection);
            [[maybe_unused]] const int kMaxSeconds = 100;  // 0 for unlimited
            // Warning: This would take several hours, commented out by default.
            // test_solving(problem, filepath, is_solvable, kMaxSeconds);
        }
    }
}
