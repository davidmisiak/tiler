#ifndef TILER_SOLVERS_SAT_UTILS_SAT_UTILS_HPP_
#define TILER_SOLVERS_SAT_UTILS_SAT_UTILS_HPP_

#include <cstdlib>
#include <vector>

namespace sat_utils {
// Represents a logical literal.
class Lit {
public:
    Lit(int x, bool is_negated = false) : x_(x), is_negated_(is_negated) {}

    inline int val() { return x_; }
    inline bool is_negated() { return is_negated_; }

    inline int to_dimacs() { return (val() + 1) * (is_negated() ? -1 : 1); }
    static inline Lit from_dimacs(int x) { return Lit(std::abs(x) - 1, x < 0); }

    inline Lit operator~() const { return Lit(x_, !is_negated_); }

private:
    int x_;
    bool is_negated_;
};

// Usually represents a disjunction of literals, but may be used as an alias for a vector of
// literals, too.
using Clause = std::vector<Lit>;

}  // namespace sat_utils

#endif  // TILER_SOLVERS_SAT_UTILS_SAT_UTILS_HPP_
