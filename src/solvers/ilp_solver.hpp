#ifndef TILER_SOLVERS_ILP_SOLVER_HPP_
#define TILER_SOLVERS_ILP_SOLVER_HPP_

#include <memory>

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/ilp_utils/coin_cbc_wrapper.hpp"
#include "solvers/solver.hpp"

class IlpSolver : public Solver {
public:
    explicit IlpSolver(Problem problem, std::unique_ptr<CoinCbcWrapper> coin_cbc_wrapper);
    Solution solve(bool print_stats = false) override;

private:
    Problem problem_;
    std::unique_ptr<CoinCbcWrapper> coin_cbc_wrapper_;
};

#endif  // TILER_SOLVERS_ILP_SOLVER_HPP_
