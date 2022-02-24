#include "solvers/sat_utils/pblib_wrapper.hpp"

#include <memory>
#include <vector>

#include "pb2cnf.h"
#include "solvers/sat_utils/sat_utils.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"

PBLibWrapper::PBLibWrapper(AMO_ENCODER::PB2CNF_AMO_Encoder amo_encoder) {
    PBConfig config = std::make_shared<PBConfigClass>();
    config->amo_encoder = amo_encoder;
    pb2cnf_ = std::make_unique<PB2CNF>(config);
}

void PBLibWrapper::at_most_one_of(const sat_utils::Clause& literals,
                                  std::unique_ptr<SatWrapper>& sat_wrapper) {
    std::vector<int> pb_literals;
    for (sat_utils::Lit lit : literals) {
        pb_literals.push_back(lit.to_dimacs());
    }
    std::vector<std::vector<int>> pb_clauses;

    int fresh_var = sat_wrapper->get_var_count();
    int new_fresh_var = pb2cnf_->encodeAtMostK(pb_literals, 1, pb_clauses,
                                               fresh_var + 1  // + 1 because of dimacs
    );
    sat_wrapper->add_var_count(new_fresh_var - fresh_var);

    for (const auto& pb_clause : pb_clauses) {
        sat_utils::Clause clause;
        for (int x : pb_clause) {
            clause.push_back(sat_utils::Lit::from_dimacs(x));
        }
        sat_wrapper->add_clause(clause);
    }
}
