#ifndef TILER_SOLVERS_SIMPLE_SOLVER_HPP_
#define TILER_SOLVERS_SIMPLE_SOLVER_HPP_

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/solver.hpp"

// The simplest solver - uses just naive backtracking.
class SimpleSolver : public Solver {
public:
    explicit SimpleSolver(Problem problem);
    Solution solve() override;

private:
    bool step();

    Problem problem_;
    Solution solution_;
};

#endif  // TILER_SOLVERS_SIMPLE_SOLVER_HPP_
