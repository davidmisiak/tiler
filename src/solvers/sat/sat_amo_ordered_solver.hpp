#ifndef TILER_SOLVERS_SAT_SAT_AMO_ORDERED_SOLVER_HPP_
#define TILER_SOLVERS_SAT_SAT_AMO_ORDERED_SOLVER_HPP_

#include <memory>

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/sat/pblib_wrapper.hpp"
#include "solvers/sat/sat_preprocessor.hpp"
#include "solvers/sat/sat_utils.hpp"
#include "solvers/sat/sat_wrapper.hpp"
#include "solvers/solver.hpp"

// Solver based on translation to a SAT problem (SAT solver selection in done through the
// sat_wrapper parameter). In addition to what SatAmoSolver does, this solver breaks tile instance
// symmetry (it orders tile instances, hence the name).
class SatAmoOrderedSolver : public Solver {
public:
    explicit SatAmoOrderedSolver(Problem problem, std::unique_ptr<SatWrapper> sat_wrapper,
                                 std::unique_ptr<SatPreprocessor> preprocessor,
                                 PBLibWrapper pblib_wrapper);

    Solution solve(bool print_stats, int max_seconds) override;

private:
    Problem problem_;
    std::unique_ptr<SatWrapper> sat_wrapper_;
    std::unique_ptr<SatPreprocessor> preprocessor_;
    PBLibWrapper pblib_wrapper_;
};

#endif  // TILER_SOLVERS_SAT_SAT_AMO_ORDERED_SOLVER_HPP_
