#include "solvers/sat_utils/pblib_wrapper.hpp"

#include <memory>
#include <vector>

#include "pb2cnf.h"
#include "solvers/sat_utils/sat_utils.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"

PBLibWrapper::PBLibWrapper(AMO_ENCODER::PB2CNF_AMO_Encoder amo_encoder,
                           AMK_ENCODER::PB2CNF_AMK_Encoder amk_encoder) {
    PBConfig config = std::make_shared<PBConfigClass>();
    config->amo_encoder = amo_encoder;
    config->amk_encoder = amk_encoder;
    pb2cnf_ = std::make_unique<PB2CNF>(config);
}

void PBLibWrapper::encode(const sat_utils::Clause& literals,
                          std::unique_ptr<SatWrapper>& sat_wrapper, int k, bool exactly_k) {
    std::vector<int64_t> pb_weights(literals.size(), 1);
    std::vector<int> pb_literals;
    for (sat_utils::Lit lit : literals) {
        pb_literals.push_back(lit.to_dimacs());
    }
    std::vector<std::vector<int>> pb_clauses;

    int fresh_var = sat_wrapper->get_var_count();
    int new_fresh_var;
    if (exactly_k) {
        new_fresh_var = pb2cnf_->encodeBoth(pb_weights, pb_literals, k, k, pb_clauses,
                                            fresh_var + 1  // + 1 because of dimacs
        );
    } else {
        new_fresh_var = pb2cnf_->encodeLeq(pb_weights, pb_literals, k, pb_clauses,
                                           fresh_var + 1  // + 1 because of dimacs
        );
    }
    sat_wrapper->add_var_count(new_fresh_var - fresh_var);

    for (const auto& pb_clause : pb_clauses) {
        sat_utils::Clause clause;
        for (int x : pb_clause) {
            clause.push_back(sat_utils::Lit::from_dimacs(x));
        }
        sat_wrapper->add_clause(clause);
    }
}
