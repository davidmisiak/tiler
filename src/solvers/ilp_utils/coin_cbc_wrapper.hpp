#ifndef TILER_SOLVERS_ILP_UTILS_COIN_CBC_WRAPPER_HPP_
#define TILER_SOLVERS_ILP_UTILS_COIN_CBC_WRAPPER_HPP_

#include <vector>

#include "coin/Cbc_C_Interface.h"
#include "solvers/ilp_utils/ilp_utils.hpp"

class CoinCbcWrapper {
public:
    CoinCbcWrapper(bool adjusted_params);
    CoinCbcWrapper(const CoinCbcWrapper&) = delete;
    CoinCbcWrapper(CoinCbcWrapper&&) = default;
    ~CoinCbcWrapper();

    inline void disableLogs() { Cbc_setLogLevel(model_, 0); }

    inline int get_var_count() const { return next_var_; }
    inline int get_constraint_count() const { return static_cast<int>(constraints_.size()); }

    inline ilp_utils::Var new_var(bool is_integer, double lower, double upper, double coeff) {
        auto var = ilp_utils::Var(next_var_++, is_integer, lower, upper, coeff);
        objective_.push_back(var);
        return var;
    }

    inline void add_constraint(const ilp_utils::Constraint& constraint) {
        constraints_.push_back(constraint);
    }

    bool solve(ilp_utils::ObjectiveSense obj_sense, double obj_limit);

    std::vector<double> get_solution();

protected:
    Cbc_Model* model_;
    int next_var_ = 0;
    ilp_utils::Vars objective_;
    std::vector<ilp_utils::Constraint> constraints_;
};

#endif  // TILER_SOLVERS_ILP_UTILS_COIN_CBC_WRAPPER_HPP_
