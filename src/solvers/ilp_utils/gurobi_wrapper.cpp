#include "solvers/ilp_utils/gurobi_wrapper.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "errors/solve_error.hpp"
#include "errors/time_limit_error.hpp"
#include "gurobi_c++.h"
#include "solvers/ilp_utils/ilp_utils.hpp"

namespace {

char get_gurobi_var_type(ilp_utils::Var var) {
    if (var.is_integer()) {
        if (var.lower() == 0.0 && var.upper() == 1.0) return GRB_BINARY;
        return GRB_INTEGER;
    }
    return GRB_CONTINUOUS;
}

int obj_sense_to_gurobi(ilp_utils::ObjectiveSense sense) {
    if (sense == ilp_utils::ObjectiveSense::kMaximize) return GRB_MAXIMIZE;
    // We deal with ObjectiveSense::kIgnore by setting objective to zero (below).
    return GRB_MINIMIZE;
}

char constraint_sense_to_gurobi(ilp_utils::ConstraintSense sense) {
    if (sense == ilp_utils::ConstraintSense::kLeq) return GRB_LESS_EQUAL;
    if (sense == ilp_utils::ConstraintSense::kGeq) return GRB_GREATER_EQUAL;
    return GRB_EQUAL;
}

}  // namespace

GurobiWrapper::GurobiWrapper(bool adjusted_params) : adjusted_params_(adjusted_params) {}

bool GurobiWrapper::solve(ilp_utils::ObjectiveSense obj_sense, double obj_limit, bool print_stats,
                          int max_seconds) {
    // Ideally, this should be in the constructor, but we receive `print_stats` only now.
    GRBEnv env = GRBEnv(true);
    env.set(GRB_IntParam_Threads, 1);
    if (!print_stats) {
        env.set(GRB_IntParam_LogToConsole, 0);
    }
    env.start();

    model_ = std::make_unique<GRBModel>(env);
    model_->set(GRB_IntParam_Seed, 1234);
    if (max_seconds) {
        model_->set(GRB_DoubleParam_TimeLimit, max_seconds);
    }
    if (adjusted_params_) {
        // These were selected empirically, but the results are not very impressive.
        model_->set(GRB_IntParam_Method, GRB_METHOD_PRIMAL);
        model_->set(GRB_IntParam_DegenMoves, 0);
        model_->set(GRB_IntParam_PreDual, 1);
    }

    for (auto var : objective_) {
        char var_type = get_gurobi_var_type(var);
        GRBVar gurobi_var = model_->addVar(var.lower(), var.upper(), var.coeff(), var_type);
        gurobi_vars_.push_back(gurobi_var);
    }
    if (obj_sense == ilp_utils::ObjectiveSense::kIgnore) {
        // Drop the objective function and use 0 instead. The objective sense should be irrelevant
        // in this case.
        model_->setObjective(GRBLinExpr{}, obj_sense_to_gurobi(obj_sense));
    } else {
        model_->set(GRB_IntAttr_ModelSense, obj_sense_to_gurobi(obj_sense));
        double relaxed_obj_limit = ilp_utils::relax_limit(obj_sense, obj_limit);
        model_->set(GRB_DoubleParam_Cutoff, relaxed_obj_limit);
        model_->set(GRB_DoubleParam_BestObjStop, relaxed_obj_limit);
    }

    for (const auto &constraint : constraints_) {
        const ilp_utils::Vars &vars = constraint.vars();
        std::vector<GRBVar> cols;
        std::transform(vars.begin(), vars.end(), std::back_inserter(cols),
                       [&](ilp_utils::Var var) { return gurobi_vars_[var.num()]; });
        std::vector<double> coeffs;
        std::transform(vars.begin(), vars.end(), std::back_inserter(coeffs),
                       [](ilp_utils::Var var) { return var.coeff(); });
        GRBLinExpr expr;
        expr.addTerms(coeffs.data(), cols.data(), static_cast<int>(cols.size()));
        char constraint_sense = constraint_sense_to_gurobi(constraint.sense());
        model_->addConstr(expr, constraint_sense, constraint.rhs());
    }

    // Uncomment when using `scripts/gurobi-tune.sh` to find the best Gurobi paramters.
    // model_->write("model.mps");
    // model_->write("model.prm");

    model_->optimize();

    int status = model_->get(GRB_IntAttr_Status);
    // TODO Should we handle INF_OR_UNBD and UNBOUNDED?
    if (status == GRB_INFEASIBLE || status == GRB_CUTOFF) return false;
    if (status == GRB_OPTIMAL || status == GRB_USER_OBJ_LIMIT) return true;
    if (max_seconds && status == GRB_TIME_LIMIT) throw TimeLimitError();
    throw SolveError("Unknown Gurobi error occured.");
}

std::vector<double> GurobiWrapper::get_solution() {
    std::vector<double> var_values;
    for (int i = 0; i < next_var_; i++) {
        var_values.push_back(gurobi_vars_[i].get(GRB_DoubleAttr_X));
    }
    return var_values;
}
