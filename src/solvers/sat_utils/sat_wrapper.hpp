#ifndef TILER_SOLVERS_SAT_UTILS_SAT_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_UTILS_SAT_WRAPPER_HPP_

#include <vector>

#include "print.hpp"
#include "solvers/sat_utils/sat_utils.hpp"

// Interaface for SAT solver wrappers. Abstracts away specific API's of SAT solvers when writing
// solving algorithms. Represents a signle CNF formula which will be solved.
class SatWrapper {
public:
    virtual ~SatWrapper() = default;

    inline int get_var_count() { return next_var_; }
    inline void add_var_count(int n) { next_var_ += n; }
    inline int get_clause_count() { return static_cast<int>(clauses_.size()); }
    inline int get_lit_count() {
        int s = 0;
        for (const auto& c : clauses_) s += static_cast<int>(c.size());
        return s;
    }

    // Create a new variable, return corresponding literal.
    inline sat_utils::Lit new_lit() { return sat_utils::Lit(next_var_++); }

    // Add a clause to the CNF formula.
    inline void add_clause(const sat_utils::Clause& clause) { clauses_.push_back(clause); }

    // Get currently present CNF clauses.
    inline std::vector<sat_utils::Clause> get_clauses() { return clauses_; }

    // Prints current info about variables and clauses.
    inline void print_stats() {
        print::stats() << get_var_count() << " variables\n"
                       << get_clause_count() << " clauses\n"
                       << get_lit_count() << " literals\n";
    }

    // Returns true if a solution exists, false if it doesn't (and throws a SolveError when the SAT
    // solver signalizes something weird). Should be called at most once.
    virtual bool solve() = 0;

    // Returns the solution model if it exists (indices are the val() values of used literals).
    // Should be called only if solve() returned true, and only once.
    virtual std::vector<bool> get_model() = 0;

protected:
    int next_var_ = 0;
    std::vector<sat_utils::Clause> clauses_;
};

#endif  // TILER_SOLVERS_SAT_UTILS_SAT_WRAPPER_HPP_
