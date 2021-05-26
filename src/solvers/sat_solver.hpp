#ifndef TILER_SOLVERS_SAT_SOLVER_HPP_
#define TILER_SOLVERS_SAT_SOLVER_HPP_

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/solver.hpp"

// Solver based on CryptoMiniSat SAT solver.
class SatSolver : public Solver {
public:
    explicit SatSolver(Problem problem);
    Solution solve() override;

private:
    Problem problem_;
    Solution solution_;
};

#endif  // TILER_SOLVERS_SAT_SOLVER_HPP_
