#include "solvers/sat/breakid_wrapper.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "breakid.hpp"
#include "print.hpp"
#include "solvers/sat/sat_utils.hpp"
#include "solvers/sat/sat_wrapper.hpp"

void BreakIDWrapper::run(std::unique_ptr<SatWrapper>& sat_wrapper,
                         [[maybe_unused]] int max_seconds) {
    breakid.set_verbosity(0);

    // pass sat_wrapper's clauses to breakid
    breakid.start_dynamic_cnf(sat_wrapper->get_var_count());
    for (const auto& clause : sat_wrapper->get_clauses()) {
        std::vector<BID::BLit> breakid_clause;
        std::transform(clause.begin(), clause.end(), std::back_inserter(breakid_clause),
                       [](sat_utils::Lit lit) { return BID::BLit(lit.val(), lit.is_negated()); });
        breakid.add_clause(breakid_clause.data(), breakid_clause.size());
    }
    breakid.end_dynamic_cnf();

    breakid.detect_subgroups();
    breakid.break_symm();

    // add the generated symmetry breaking clauses to sat_wrapper
    for (const auto& breakid_clause : breakid.get_brk_cls()) {
        sat_utils::Clause clause;
        std::transform(breakid_clause.begin(), breakid_clause.end(), std::back_inserter(clause),
                       [](BID::BLit blit) { return sat_utils::Lit(blit.var(), blit.sign()); });
        sat_wrapper->add_clause(clause);
    }

    sat_wrapper->add_var_count(breakid.get_num_aux_vars());
}

void BreakIDWrapper::print_stats() {
    print::stats() << breakid.get_num_aux_vars() << " breakid aux variables\n"
                   << breakid.get_num_break_cls() << " breakid aux clauses\n";
}
