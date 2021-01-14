#ifndef TILER_SOLVERS_SIMPLE_SOLVER_HPP_
#define TILER_SOLVERS_SIMPLE_SOLVER_HPP_

#include <string>
#include <vector>

#include "problem/problem.hpp"

// The simplest solver - uses just naive backtracking.
class SimpleSolver {
public:
    SimpleSolver() = delete;
    explicit SimpleSolver(Problem problem);
    bool solve();

private:
    Problem problem_;
};

#endif  // TILER_SOLVERS_SIMPLE_SOLVER_HPP_
