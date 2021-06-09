#ifndef TILER_SOLVERS_SOLVER_HPP_
#define TILER_SOLVERS_SOLVER_HPP_

#include "solution/solution.hpp"

// The interface of solvers.
class Solver {
public:
    virtual ~Solver() = default;

    // Solves the problem. Should be called at most once.
    virtual Solution solve(bool print_stats = false) = 0;
};

#endif  // TILER_SOLVERS_SOLVER_HPP_
