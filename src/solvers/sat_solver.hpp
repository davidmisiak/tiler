#ifndef TILER_SOLVERS_SAT_SOLVER_HPP_
#define TILER_SOLVERS_SAT_SOLVER_HPP_

#include <cryptominisat5/cryptominisat.h>

#include <vector>

#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/solver.hpp"

// Solver based on CryptoMiniSat SAT solver.
class SatSolver : public Solver {
public:
    explicit SatSolver(Problem problem);
    Solution solve() override;

private:
    using Clause = std::vector<CMSat::Lit>;

    // Adds clauses that guarantee that at most on of `literals` will be true. Modifies `next_var_`.
    void at_most_one_of(Clause literals);

    Problem problem_;
    unsigned int next_var_ = 0;
    std::vector<Clause> clauses_;
};

#endif  // TILER_SOLVERS_SAT_SOLVER_HPP_
