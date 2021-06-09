#ifndef TILER_SOLVERS_SAT_SOLVER_HPP_
#define TILER_SOLVERS_SAT_SOLVER_HPP_

#include <memory>

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/sat_utils/sat_utils.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"
#include "solvers/solver.hpp"

// Solver based on translation to a SAT problem (SAT solver selection in done through the
// sat_wrapper parameter).
class SatSolver : public Solver {
public:
    explicit SatSolver(Problem problem, std::unique_ptr<SatWrapper> sat_wrapper);
    Solution solve(bool print_stats = false) override;

private:
    // Adds clauses that guarantee that at most one of `literals` will be true. Creates new utility
    // variables.
    void at_most_one_of(sat_utils::Clause literals);

    Problem problem_;
    std::unique_ptr<SatWrapper> sat_wrapper_;
};

#endif  // TILER_SOLVERS_SAT_SOLVER_HPP_
