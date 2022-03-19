#ifndef TILER_SOLVERS_ILP_UTILS_ILP_UTILS_HPP_
#define TILER_SOLVERS_ILP_UTILS_ILP_UTILS_HPP_

#include <cstdlib>
#include <vector>

#include "coin/CoinFinite.hpp"

namespace ilp_utils {

const double kMinDouble = COIN_DBL_MIN;
const double kMaxDouble = COIN_DBL_MAX;
const double kEps = 1e-5;

// Constraint sense - whether the linear combination of variables should be leq, geq or eq to the
// constraint's rhs.
enum class ConstraintSense {
    kLeq = 'L',
    kGeq = 'G',
    kEq = 'E',
};

// Objective sense - whetner we are minimizing or maximizing the objective function, or ignoring it
// (when we are concerned only with feasibility).
enum class ObjectiveSense {
    kMinimize = 1,
    kMaximize = -1,
    kIgnore = 0,
};

// ILP variable. Note that this class represents not only the variable itself (its number, bounds
// and whether it is an integer), but also a coefficient associated with it (variables in
// constraints as well as in the objective function always carry a coeffcient, so it is easier to
// bundle them this way.)
class Var {
public:
    Var(int num, bool is_integer, double lower, double upper, double coeff)
            : num_(num), is_integer_(is_integer), lower_(lower), upper_(upper), coeff_(coeff) {}

    // Returns the same variable as `this`, but with the given `coef`.
    inline Var with_coeff(double coeff) const {
        Var new_var(*this);
        new_var.coeff_ = coeff;
        return new_var;
    }

    inline int num() const { return num_; }
    inline bool is_integer() const { return is_integer_; }
    inline double lower() const { return lower_; }
    inline double upper() const { return upper_; }
    inline double coeff() const { return coeff_; }

private:
    int num_;
    bool is_integer_;
    double lower_;
    double upper_;
    double coeff_;
};

// Alias for a list of variables (and their coefficients).
using Vars = std::vector<Var>;

// ILP constraint.
class Constraint {
public:
    Constraint(Vars vars, ConstraintSense sense, double rhs)
            : vars_(vars), sense_(sense), rhs_(rhs) {}

    inline const Vars& vars() const { return vars_; }
    inline ConstraintSense sense() const { return sense_; }
    inline double rhs() const { return rhs_; }

private:
    Vars vars_;
    ConstraintSense sense_;
    double rhs_;
};

inline bool approxEq(double lhs, double rhs) { return std::abs(lhs - rhs) < kEps; }
inline bool approxLeq(double lhs, double rhs) { return lhs < rhs + kEps; }
inline bool approxGeq(double lhs, double rhs) { return lhs > rhs - kEps; }

// Decides whether the objective function `result` passes the `limit` with regards to the objective
// `sense`.
inline bool evaluate_obj_result(ObjectiveSense sense, double result, double limit) {
    if (sense == ObjectiveSense::kMinimize) return approxLeq(result, limit);
    if (sense == ObjectiveSense::kMaximize) return approxGeq(result, limit);
    return true;  // ObjectiveSense::kIgnore
}

// Returns the value of `limit` shifted by `kEps` generously to the direction determined by the
// objective `sense`.
inline double relax_limit(ObjectiveSense sense, double limit) {
    if (sense == ObjectiveSense::kMinimize) return limit + kEps;
    if (sense == ObjectiveSense::kMaximize) return limit - kEps;
    return limit;  // ObjectiveSense::kIgnore
}

}  // namespace ilp_utils

#endif  // TILER_SOLVERS_ILP_UTILS_ILP_UTILS_HPP_
