#include "solvers/sat/kissat_wrapper.hpp"

#include <csignal>
#include <vector>

#include "errors/solve_error.hpp"
#include "errors/time_limit_error.hpp"
#include "solvers/sat/sat_utils.hpp"

namespace {

static kissat* solver_to_terminate_;

static void catch_alarm_([[maybe_unused]] int sig) { kissat_terminate(solver_to_terminate_); }

}  // namespace

KissatWrapper::KissatWrapper() { solver_ = kissat_init(); }

KissatWrapper::~KissatWrapper() { kissat_release(solver_); }

bool KissatWrapper::solve(int max_seconds) {
    kissat_set_option(solver_, "quiet", 1);  // in case Kissat was build without the QUIET flag

    if (max_seconds) {
        solver_to_terminate_ = solver_;
        signal(SIGALRM, catch_alarm_);
        alarm(max_seconds);
    }

    for (const sat_utils::Clause& clause : clauses_) {
        for (sat_utils::Lit lit : clause) {
            kissat_add(solver_, lit.to_dimacs());
        }
        kissat_add(solver_, 0);
    }

    int result = kissat_solve(solver_);

    if (max_seconds) {
        signal(SIGALRM, SIG_DFL);
        alarm(0);  // cancel the alarm
    }

    if (result == 20) return false;
    if (result == 10) return true;
    if (max_seconds) throw TimeLimitError();
    throw SolveError("Unknown Kissat error occured.");
}

std::vector<bool> KissatWrapper::get_model() {
    std::vector<bool> model;
    for (int i = 0; i < next_var_; i++) {
        model.push_back(kissat_value(solver_, i + 1) > 0);
    }
    return model;
}
