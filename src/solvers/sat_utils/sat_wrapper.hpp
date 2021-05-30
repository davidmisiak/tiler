#ifndef TILER_SOLVERS_SAT_UTILS_SAT_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_UTILS_SAT_WRAPPER_HPP_

#include <vector>

#include "solvers/sat_utils/sat_utils.hpp"

// Interaface for SAT solver wrappers. Abstracts away specific API's of SAT solvers when writing
// solving algorithms.
class SatWrapper {
public:
    virtual ~SatWrapper() = default;

    inline sat_utils::Lit new_lit() { return sat_utils::Lit(next_var_++); }
    inline void add_clause(sat_utils::Clause clause) { clauses_.push_back(clause); }

    // Resets the wrapper to the initial state.
    inline virtual void clear() {
        next_var_ = 0;
        clauses_.clear();
    }

    // Returns true if a solution exists, false if it doesn't (and throws a SolveError when the SAT
    // solver signalizes something weird). Should be called at most once (or call clear()).
    virtual bool solve() = 0;

    // Returns the solution model if it exists (indices are the val() values of used literals).
    // Should be called only if solve() returned true, and only once.
    virtual std::vector<bool> get_model() = 0;

protected:
    int next_var_ = 0;
    std::vector<sat_utils::Clause> clauses_;
};

#endif  // TILER_SOLVERS_SAT_UTILS_SAT_WRAPPER_HPP_
