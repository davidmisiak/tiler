#ifndef TILER_SOLVERS_ILP_ILP_WRAPPER_HPP_
#define TILER_SOLVERS_ILP_ILP_WRAPPER_HPP_

#include <vector>

#include "solvers/ilp/ilp_utils.hpp"

// Interaface for ILP solver wrappers. Abstracts away specific API's of ILP solvers when writing
// solving algorithms. Represents a single linear problem which will be optimzed.
class IlpWrapper {
public:
    virtual ~IlpWrapper() = default;

    inline int get_var_count() const { return next_var_; }
    inline int get_constraint_count() const { return static_cast<int>(constraints_.size()); }

    // Adds a new variable with `lower` and `upper` bounds. The `coeff` is used as the varible's
    // coefficient in the problem objective function. The created variable is returned with the
    // given coeffcient as well.
    inline ilp_utils::Var new_var(bool is_integer, double lower, double upper, double coeff) {
        auto var = ilp_utils::Var(next_var_++, is_integer, lower, upper, coeff);
        objective_.push_back(var);
        return var;
    }

    // Adds a constraint.
    inline void add_constraint(const ilp_utils::Constraint& constraint) {
        constraints_.push_back(constraint);
    }

    // Returns true if the ILP is feasible and the objective value complies with the `obj_limit`,
    // false otherwise (and throws a SolveError when the ILP solver signalizes something weird).
    // Should be called at most once.
    virtual bool solve(ilp_utils::ObjectiveSense obj_sense, double obj_limit, bool print_stats,
                       int max_seconds) = 0;

    // Returns the optimal variable values if the ILP is feasible (indices are the num() values of
    // the used variables). Should be called only if solve() returned true, and only once.
    virtual std::vector<double> get_solution() = 0;

protected:
    int next_var_ = 0;
    ilp_utils::Vars objective_;
    std::vector<ilp_utils::Constraint> constraints_;
};

#endif  // TILER_SOLVERS_ILP_ILP_WRAPPER_HPP_
