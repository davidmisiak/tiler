#include "solvers/sat/sbva_wrapper.hpp"

#include <algorithm>
#include <memory>
#include <vector>

// #include "breakid.hpp"
#include "print.hpp"
#include "solvers/sat/sat_utils.hpp"
#include "solvers/sat/sat_wrapper.hpp"

extern std::pair<int, std::vector<std::vector<int>>> runBVA(
        const std::vector<std::vector<int>>& cnf, int max_seconds);

void SbvaWrapper::run(std::unique_ptr<SatWrapper>& sat_wrapper, int max_seconds) {
    // convert sat_utils::Clause to DIMACS input for SBVA
    std::vector<std::vector<int>> sbva_clauses;
    for (const auto& clause : sat_wrapper->get_clauses()) {
        std::vector<int> sbva_clause;
        std::transform(clause.begin(), clause.end(), std::back_inserter(sbva_clause),
                       [](sat_utils::Lit lit) { return lit.to_dimacs(); });
        sbva_clauses.push_back(sbva_clause);
    }

    auto [vars, result] = runBVA(sbva_clauses, max_seconds);

    // convert SBVA result back to sat_utils::Clause
    std::vector<sat_utils::Clause> new_clauses;
    for (const auto& sbva_clause : result) {
        sat_utils::Clause clause;
        std::transform(sbva_clause.begin(), sbva_clause.end(), std::back_inserter(clause),
                       [](int lit) { return sat_utils::Lit::from_dimacs(lit); });
        new_clauses.push_back(clause);
    }

    added_vars_ = vars - sat_wrapper->get_var_count();
    removed_clauses_ = static_cast<int>(sbva_clauses.size()) - static_cast<int>(new_clauses.size());
    sat_wrapper->add_var_count(added_vars_);
    sat_wrapper->set_clauses(new_clauses);
}

void SbvaWrapper::print_stats() {
    print::stats() << added_vars_ << " sbva aux variables\n"
                   << removed_clauses_ << " sbva removed clauses\n";
}
