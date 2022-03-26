#ifndef TILER_SOLVERS_SAT_PBLIB_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_PBLIB_WRAPPER_HPP_

#include <memory>
#include <vector>

#include "pb2cnf.h"
#include "solvers/sat/sat_utils.hpp"
#include "solvers/sat/sat_wrapper.hpp"

// PBLib wrapper. Takes care of translating between our types and PBLib types when encoding
// pseudo-boolean constraints.
class PBLibWrapper {
public:
    PBLibWrapper(AMO_ENCODER::PB2CNF_AMO_Encoder amo_encoder,
                 AMK_ENCODER::PB2CNF_AMK_Encoder amk_encoder);

    // Adds clauses that guarantee that at most k of `literals` will be true. Creates new utility
    // variables.
    inline void at_most_k(const sat_utils::Clause& literals,
                          std::unique_ptr<SatWrapper>& sat_wrapper, int k) {
        return encode(literals, sat_wrapper, k, false);
    }

    // Adds clauses that guarantee that exactly k of `literals` will be true. Creates new utility
    // variables.
    inline void exactly_k(const sat_utils::Clause& literals,
                          std::unique_ptr<SatWrapper>& sat_wrapper, int k) {
        return encode(literals, sat_wrapper, k, true);
    }

private:
    void encode(const sat_utils::Clause& literals, std::unique_ptr<SatWrapper>& sat_wrapper, int k,
                bool exactly_k);

    std::unique_ptr<PB2CNF> pb2cnf_;
};

#endif  // TILER_SOLVERS_SAT_PBLIB_WRAPPER_HPP_
