#ifndef TILER_SOLVERS_DLX_DLX_SOLVER_HPP_
#define TILER_SOLVERS_DLX_DLX_SOLVER_HPP_

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/dlx/dlx_wrapper.hpp"
#include "solvers/solver.hpp"

// Solver based on translation to an exact cover problem. Employs Knuth's DLX algorithm.
class DlxSolver : public Solver {
public:
    explicit DlxSolver(Problem problem, DlxWrapper dlx_wrapper);

    Solution solve(bool print_stats, int max_seconds) override;

private:
    Problem problem_;
    DlxWrapper dlx_wrapper_;
};

#endif  // TILER_SOLVERS_DLX_DLX_SOLVER_HPP_
