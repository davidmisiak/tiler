#ifndef TILER_SOLVERS_ILP_SOLVER_HPP_
#define TILER_SOLVERS_ILP_SOLVER_HPP_

#include <memory>

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/ilp_utils/coin_cbc_wrapper.hpp"
#include "solvers/ilp_utils/ilp_utils.hpp"
#include "solvers/solver.hpp"

class IlpSolver : public Solver {
public:
    explicit IlpSolver(Problem problem, std::unique_ptr<CoinCbcWrapper> coin_cbc_wrapper,
                       ilp_utils::ConstraintSense cell_sense, ilp_utils::ObjectiveSense obj_sense);
    Solution solve(bool print_stats = false) override;

private:
    Problem problem_;
    std::unique_ptr<CoinCbcWrapper> coin_cbc_wrapper_;
    ilp_utils::ConstraintSense cell_sense_;
    ilp_utils::ObjectiveSense obj_sense_;
};

#endif  // TILER_SOLVERS_ILP_SOLVER_HPP_
