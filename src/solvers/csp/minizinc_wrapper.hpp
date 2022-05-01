#ifndef TILER_SOLVERS_CSP_MINIZINC_WRAPPER_HPP_
#define TILER_SOLVERS_CSP_MINIZINC_WRAPPER_HPP_

#include <set>
#include <string>
#include <vector>

#include "print.hpp"

// Wrapper for the MiniZinc CSP toolkit. Takes care of translating the input and output C++ data
// structures specific for our approach for the MiniZinc API. Represents a single constraint
// satisfaction problem which will be solved.
class MinizincWrapper {
public:
    using Positions = std::vector<std::pair<int, int>>;
    using Kinds = std::vector<int>;

    // The `flat_solver` must be a valid solver name accepted by MiniZinc (e.g. "Gecode").
    MinizincWrapper(std::string flat_solver);

    // Solves a single tiling instance. If no solution exists, positions and kinds will be empty;
    // if a solution exists, the values produced by our MiniZinc model will be returned.
    // If something weird happens, a SolveError is thrown. Should be called at most once.
    std::pair<Positions, Kinds> solve(int w, int h, Positions missingCells,
                                      std::vector<std::set<int>> variants,
                                      std::vector<std::set<int>> tiles, bool print_stats,
                                      int max_seconds);

private:
    std::string flat_solver_;
};

#endif  // TILER_SOLVERS_CSP_MINIZINC_WRAPPER_HPP_
