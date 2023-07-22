#ifndef TILER_SOLVERS_SIMPLE_SIMPLE_SOLVER_HPP_
#define TILER_SOLVERS_SIMPLE_SIMPLE_SOLVER_HPP_

#include <cstdint>
#include <vector>

#include "problem/problem.hpp"
#include "problem/region.hpp"
#include "solution/solution.hpp"
#include "solvers/simple/variant_ordering.hpp"
#include "solvers/solver.hpp"

// The simplest solver - uses backtracking with basic tile variant choice heursitics.
class SimpleSolver : public Solver {
public:
    explicit SimpleSolver(Problem problem, variant_ordering::Ordering ordering);

    Solution solve(bool print_stats, int max_seconds) override;

private:
    bool step();

    Problem problem_;
    std::vector<variant_ordering::VariantRef> variant_refs_;
    variant_ordering::Ordering ordering_;
    Solution solution_;
    // number of visited search tree nodes (used for stats)
    uint64_t nodes_ = 0;
};

#endif  // TILER_SOLVERS_SIMPLE_SIMPLE_SOLVER_HPP_
