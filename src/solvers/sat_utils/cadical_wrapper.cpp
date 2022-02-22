#include "solvers/sat_utils/cadical_wrapper.hpp"

#include <vector>

#include "solve_error.hpp"
#include "solvers/sat_utils/sat_utils.hpp"

bool CadicalWrapper::solve() {
    solver_.set("quiet", 1);  // silence some log messages printed to stdout

    for (const sat_utils::Clause& clause : clauses_) {
        for (sat_utils::Lit lit : clause) {
            int n = lit.val() + 1;
            solver_.add(lit.is_negated() ? -n : n);
        }
        solver_.add(0);
    }

    int result = solver_.solve();
    if (result == 20) return false;
    if (result == 10) return true;
    throw SolveError("Unknown CaDiCaL error occured.");
}

std::vector<bool> CadicalWrapper::get_model() {
    std::vector<bool> model;
    for (int i = 0; i < next_var_; i++) {
        model.push_back(solver_.val(i + 1) > 0);
    }
    return model;
}
