#ifndef TILER_SOLVERS_SOLVER_HPP_
#define TILER_SOLVERS_SOLVER_HPP_

#include "solution/solution.hpp"

// The interface of solvers.
class Solver {
public:
    virtual Solution solve() = 0;
    virtual ~Solver() = default;
};

#endif  // TILER_SOLVERS_SOLVER_HPP_
