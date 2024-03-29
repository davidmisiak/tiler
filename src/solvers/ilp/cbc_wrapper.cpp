#include "solvers/ilp/cbc_wrapper.hpp"

#include <algorithm>
#include <vector>

#include "coin/Cbc_C_Interface.h"
#include "errors/solve_error.hpp"
#include "errors/time_limit_error.hpp"
#include "solvers/ilp/ilp_utils.hpp"

CbcWrapper::CbcWrapper(bool adjusted_params) {
    model_ = Cbc_newModel();

    Cbc_setParameter(model_, "randomSeed", "1234");
    Cbc_setParameter(model_, "randomCbcSeed", "5678");

    if (adjusted_params) {
        // These were selected empirically, but the results are not very impressive.
        Cbc_setParameter(model_, "feasibilityPump", "off");
        Cbc_setParameter(model_, "cutsOnOff", "off");
        Cbc_setParameter(model_, "cliqueCuts", "on");
        Cbc_setParameter(model_, "heuristicsOnOff", "off");
        Cbc_setParameter(model_, "greedyHeuristic", "on");
    }
}

CbcWrapper::~CbcWrapper() { Cbc_deleteModel(model_); }

bool CbcWrapper::solve(ilp_utils::ObjectiveSense obj_sense, double obj_limit, bool print_stats,
                       int max_seconds) {
    if (!print_stats) {
        Cbc_setLogLevel(model_, 0);
    }
    if (max_seconds) {
        Cbc_setMaximumSeconds(model_, max_seconds);
    }

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
            return obj_sense == ilp_utils::ObjectiveSense::kIgnore ||
                   ilp_utils::evaluate_obj_result(obj_sense, Cbc_getObjValue(model_), obj_limit);
        }
    }
    if (max_seconds && Cbc_status(model_) == 1 && Cbc_secondaryStatus(model_) == 4) {
        throw TimeLimitError();
    }
    throw SolveError("Unknown CBC error occured.");
}

std::vector<double> CbcWrapper::get_solution() {
    const double *cbc_var_values = Cbc_getColSolution(model_);
    std::vector<double> var_values(cbc_var_values, cbc_var_values + objective_.size());
    return var_values;
}
