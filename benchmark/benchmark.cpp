#include "benchmark/benchmark.h"

#include <iostream>

#include "parsers/problem_parser.hpp"
#include "problem/problem.hpp"
#include "solvers/solver_factory.hpp"

auto solve = [](benchmark::State& state, Problem problem, std::string solver_name) {
    for (auto _ : state) {
        solver_factory::create(solver_name, problem)->solve();
    }
};

int main(int argc, char** argv) {
    std::string filepath;
    while (std::cin >> filepath) {
        Problem problem = problem_parser::parse_from_file(filepath, false);
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
