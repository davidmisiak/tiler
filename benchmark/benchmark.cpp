#include "benchmark/benchmark.h"

static void benchmark_solve(benchmark::State& state) {
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        int i = 1 + 1;
    }
}

BENCHMARK(benchmark_solve);

BENCHMARK_MAIN();
