#include "solvers/ilp_utils/coin_cbc_wrapper.hpp"

#include <algorithm>
#include <vector>

#include "coin/Cbc_C_Interface.h"
#include "solve_error.hpp"
#include "solvers/ilp_utils/ilp_utils.hpp"

bool CoinCbcWrapper::solve(ilp_utils::ObjectiveSense obj_sense, double obj_limit) {
    Cbc_setObjSense(model_, static_cast<double>(obj_sense));

    for (auto var : objective_) {
        Cbc_addCol(model_, "", var.lower(), var.upper(), var.coeff(), var.is_integer(), 0, nullptr,
                   nullptr);
    }

    for (const auto &constraint : constraints_) {
        const ilp_utils::Vars &vars = constraint.vars();
        std::vector<int> cols;
        std::transform(vars.begin(), vars.end(), std::back_inserter(cols),
                       [](ilp_utils::Var var) { return var.num(); });
        std::vector<double> coeffs;
        std::transform(vars.begin(), vars.end(), std::back_inserter(coeffs),
                       [](ilp_utils::Var var) { return var.coeff(); });
        Cbc_addRow(model_, "", static_cast<int>(vars.size()), cols.data(), coeffs.data(),
                   static_cast<char>(constraint.sense()), constraint.rhs());
    }

    if (Cbc_solve(model_) == 0) {
        if (Cbc_isProvenInfeasible(model_)) return false;
        if (Cbc_isProvenOptimal(model_)) {
            return ilp_utils::evaluate_obj_result(obj_sense, Cbc_getObjValue(model_), obj_limit);
        }
    }
    // uncomment when timeout is active and the result is not important (e.g. when benchmarking)
    // return false;
    throw SolveError("Unknown COIN-CBC error occured.");
}

std::vector<double> CoinCbcWrapper::get_solution() {
    const double *cbc_var_values = Cbc_getColSolution(model_);
    std::vector<double> var_values(cbc_var_values, cbc_var_values + objective_.size());
    return var_values;
}
