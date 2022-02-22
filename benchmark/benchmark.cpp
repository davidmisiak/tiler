#include "benchmark/benchmark.h"

#include <string>

#include "parsers/problem_parser.hpp"
#include "problem/problem.hpp"
#include "solvers/solver_factory.hpp"

int main(int argc, char** argv) {
    auto solve = [](benchmark::State& state, Problem problem, std::string solver_name) {
        for (auto _ : state) {
            solver_factory::create(solver_name, problem)->solve();
        }
    };

    for (std::string filepath : utils::get_file_paths("problems")) {
        bool reflection = filepath.find('\'') != std::string::npos;
        Problem problem = problem_parser::parse_from_file(filepath, reflection);

        for (auto solver_name : solver_factory::solver_names) {
            std::string name = filepath + "@" + solver_name;
            benchmark::RegisterBenchmark(name.c_str(), solve, problem, solver_name)
                    ->Unit(benchmark::kMillisecond);
        }
    }

    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    benchmark::RunSpecifiedBenchmarks();
}
