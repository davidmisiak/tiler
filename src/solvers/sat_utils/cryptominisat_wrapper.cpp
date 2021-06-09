#include "solvers/sat_utils/cryptominisat_wrapper.hpp"

#include <algorithm>
#include <vector>

#include "cryptominisat5/cryptominisat.h"
#include "solve_error.hpp"
#include "solvers/sat_utils/sat_utils.hpp"

bool CryptominisatWrapper::solve() {
    solver_.new_vars(next_var_);

    for (sat_utils::Clause clause : clauses_) {
        std::vector<CMSat::Lit> cmsat_clause;
        std::transform(clause.begin(), clause.end(), std::back_inserter(cmsat_clause),
                       [&](sat_utils::Lit lit) { return CMSat::Lit(lit.val(), lit.is_negated()); });
        solver_.add_clause(cmsat_clause);
    }

    CMSat::lbool result = solver_.solve();
    if (result == CMSat::l_False) return false;
    if (result == CMSat::l_True) return true;
    throw SolveError("Unknown CryptoMiniSat error occured.");
}

std::vector<bool> CryptominisatWrapper::get_model() {
    std::vector<bool> model;
    std::vector<CMSat::lbool> cmsat_model = solver_.get_model();
    for (int i = 0; i < next_var_; i++) {
        model.push_back(cmsat_model[i] == CMSat::l_True);
    }
    return model;
}
