
#include "solvers/csp/minizinc_wrapper.hpp"

#include <algorithm>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "errors/solve_error.hpp"
#include "errors/time_limit_error.hpp"
#include "minizinc/solver.hh"
#include "nlohmann/json.hpp"
#include "print.hpp"
#include "solvers/csp/minizinc_models.hpp"

namespace {

nlohmann::json serialize_sets(std::vector<std::set<int>> sets) {
    nlohmann::json j = {};
    for (const auto& s : sets) {
        j.push_back({{"set", s}});
    }
    return j;
}

}  // namespace

MinizincWrapper::MinizincWrapper(std::string flat_solver) : flat_solver_(flat_solver){};

std::pair<MinizincWrapper::Positions, MinizincWrapper::Kinds> MinizincWrapper::solve(
        int w, int h, std::vector<std::pair<int, int>> missingCells,
        std::vector<std::set<int>> variants, std::vector<std::set<int>> tiles, bool print_stats,
        int max_seconds) {
    nlohmann::json input_json = {
            {"w", w},
            {"h", h},
            {"nEmpty", missingCells.size()},
            {"nVariants", variants.size()},
            {"nTiles", tiles.size()},
            {"empty", missingCells},
            {"variants", serialize_sets(variants)},
            {"tiles", serialize_sets(tiles)},
    };

    // Uncomment if you want to print the covering problem data in MiniZinc-compatible JSON format.
    // print::normal() << input_json << std::endl;

    auto MinizincError = SolveError("Unknown MiniZinc error occured.");

    std::stringstream output_stream;
    MiniZinc::SolverInstance::Status status;
    try {
        MiniZinc::Timer startTime;
        MiniZinc::MznSolver solver(output_stream, print::stats(), startTime);
        std::vector<std::string> args{
                "--json-stream", "--cmdline-json-data", input_json.dump(), "--output-mode", "json",
                "--solver",      flat_solver_,
        };
        if (print_stats) {
            args.push_back("-v");
        }
        if (max_seconds) {
            args.push_back("--time-limit");
            args.push_back(std::to_string(max_seconds * 1000));
        }
        status = solver.run(args, minizinc_models::kMinizincModel);
    } catch (...) {
        throw MinizincError;
    }

    if (max_seconds && status == MiniZinc::SolverInstance::Status::UNKNOWN) throw TimeLimitError();

    if (status == MiniZinc::SolverInstance::Status::UNSAT) return {{}, {}};

    if (status != MiniZinc::SolverInstance::Status::SAT) throw MinizincError;
    nlohmann::json output_json;
    output_stream >> output_json;
    if (output_json["type"] != "solution") throw MinizincError;

    Positions positions;
    for (std::vector<int> p : output_json["output"]["json"]["positions"]) {
        positions.push_back({p[0], p[1]});
    }
    Kinds kinds = output_json["output"]["json"]["kinds"];
    return {positions, kinds};
}
