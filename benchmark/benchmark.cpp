#include "benchmark/benchmark.h"

#include <string>

#include "errors/not_implemented_error.hpp"
#include "errors/time_limit_error.hpp"
#include "parsers/problem_parser.hpp"
#include "problem/problem.hpp"
#include "solvers/solver_factory.hpp"

int main(int argc, char** argv) {
    const int kMaxSeconds = 1000;  // 0 for unlimited

    auto solve = [](benchmark::State& state, const Problem& problem,
                    const std::string& solver_name) {
        for (auto _ : state) {
            try {
                solver_factory::create(solver_name, problem)->solve(false, kMaxSeconds);
            } catch (const TimeLimitError& e) {
                // Intentionally empty - we handle time limit overruns in benchmark.py.
            } catch (const NotImplementedError& e) {
                // We re-throw this error so that the operator can filter out the offending
                // problems. Other options would be silently accepting (but that might hide the fact
                // that no solving was actually performed) or waiting `kMaxSeconds` (but that would
                // greatly prolong the benchmark run time). Apparently, there is no simple way how
                // to set the iteration time manually only for a selection of iteration runs.
                throw e;
            }
        }
    };

    for (const std::string& filepath : utils::get_file_paths("problems")) {
        bool reflection = filepath.find('\'') != std::string::npos;
        Problem problem = problem_parser::parse_from_file(filepath, reflection);

        for (const std::string& solver_name : solver_factory::get_solver_names()) {
            std::string name = filepath + "@" + solver_name;
            benchmark::RegisterBenchmark(name.c_str(), solve, problem, solver_name)
                    ->Unit(benchmark::kMillisecond);
        }
    }

    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    benchmark::RunSpecifiedBenchmarks();
}
