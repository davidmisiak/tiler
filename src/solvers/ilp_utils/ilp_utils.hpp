#ifndef TILER_SOLVERS_ILP_UTILS_ILP_UTILS_HPP_
#define TILER_SOLVERS_ILP_UTILS_ILP_UTILS_HPP_

#include <vector>

#include "coin/CoinFinite.hpp"

namespace ilp_utils {

const double kMinDouble = COIN_DBL_MIN;
const double kMaxDouble = COIN_DBL_MAX;

enum class ConstraintSense {
    kLeq = 'L',
    kGeq = 'G',
    kEq = 'E',
};

enum class ObjectiveSense {
    kMinimize = 1,
    kMaximize = -1,
    kIgnore = 0,
};

class Var {
public:
    Var(int num, bool is_integer, double lower, double upper, double coeff)
            : num_(num), is_integer_(is_integer), lower_(lower), upper_(upper), coeff_(coeff) {}

    inline Var with_coeff(double coeff) const {
        Var new_var(*this);
        new_var.coeff_ = coeff;
        return new_var;
    }

    inline int num() const { return num_; }
    inline bool is_integer() const { return is_integer_; }
    inline double lower() const { return lower_; }
    inline double upper() const { return upper_; }
    inline double coeff() const { return upper_; }

private:
    int num_;
    bool is_integer_;
    double lower_;
    double upper_;
    double coeff_;
};

using Vars = std::vector<Var>;

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

}  // namespace ilp_utils

#endif  // TILER_SOLVERS_ILP_UTILS_ILP_UTILS_HPP_
