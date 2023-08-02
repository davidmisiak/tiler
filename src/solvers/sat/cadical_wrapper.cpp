#include "solvers/sat/cadical_wrapper.hpp"

#include <csignal>
#include <vector>

#include "errors/solve_error.hpp"
#include "errors/time_limit_error.hpp"
#include "signal.hpp"
#include "solvers/sat/sat_utils.hpp"

bool CadicalWrapper::solve(int max_seconds) {
    solver_.set("quiet", 1);  // in case CaDiCaL was build without the QUIET flag
    solver_.set("seed", 1234);

    if (max_seconds) {
        CaDiCaL::Signal::set(this);  // `this` implements CaDiCaL::Handler interface
        CaDiCaL::Signal::alarm(max_seconds);
    }

    for (const sat_utils::Clause& clause : clauses_) {
        for (sat_utils::Lit lit : clause) {
            solver_.add(lit.to_dimacs());
        }
        solver_.add(0);
    }

    int result = solver_.solve();

    if (max_seconds) {
        CaDiCaL::Signal::alarm(0);  // cancel the alarm
        CaDiCaL::Signal::reset();
    }

    if (result == 20) return false;
    if (result == 10) return true;
    if (max_seconds) throw TimeLimitError();
    throw SolveError("Unknown CaDiCaL error occured.");
}

std::vector<bool> CadicalWrapper::get_model() {
    std::vector<bool> model;
    for (int i = 0; i < next_var_; i++) {
        model.push_back(solver_.val(i + 1) > 0);
    }
    return model;
}
