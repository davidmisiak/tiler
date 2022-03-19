#ifndef TILER_SOLVERS_ILP_SOLVER_HPP_
#define TILER_SOLVERS_ILP_SOLVER_HPP_

#include <memory>

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/ilp_utils/ilp_utils.hpp"
#include "solvers/ilp_utils/ilp_wrapper.hpp"
#include "solvers/solver.hpp"

// Solver based on translation to an ILP problem. The tranlsation is similar to the one utilized by
// SatAmkSolver.
class IlpSolver : public Solver {
public:
    // The `cell_sense` is used for encoding the "at-most/at-least/exactly one tile instance
    // covering each cell" constraints. `obj_sense` decides whether we minimize/maximize/ignore the
    // tile instance cells placed on the board.
    explicit IlpSolver(Problem problem, std::unique_ptr<IlpWrapper> ilp_wrapper,
                       ilp_utils::ConstraintSense cell_sense, ilp_utils::ObjectiveSense obj_sense);

    Solution solve(bool print_stats, int max_seconds) override;

private:
    Problem problem_;
    std::unique_ptr<IlpWrapper> ilp_wrapper_;
    ilp_utils::ConstraintSense cell_sense_;
    ilp_utils::ObjectiveSense obj_sense_;
};

#endif  // TILER_SOLVERS_ILP_SOLVER_HPP_
