#ifndef TILER_SOLVERS_CSP_CSP_SOLVER_HPP_
#define TILER_SOLVERS_CSP_CSP_SOLVER_HPP_

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/csp/minizinc_wrapper.hpp"
#include "solvers/solver.hpp"

// Solver based on translation to a MiniZinc-compatible constraint satisfaction problem. See
// `minizinc_models::kMinizincModel` for the model source code. This class provides the appropriate
// input data for the MiniZinc model and interprets MiniZinc's solving output. Currently, only
// problems with no extra tile instances are supported (otherwise, NotImplementedError is thrown).
class CspSolver : public Solver {
public:
    explicit CspSolver(Problem problem, MinizincWrapper minizinc_wrapper);

    Solution solve(bool print_stats, int max_seconds) override;

private:
    Problem problem_;
    MinizincWrapper minizinc_wrapper_;
};

#endif  // TILER_SOLVERS_CSP_CSP_SOLVER_HPP_
