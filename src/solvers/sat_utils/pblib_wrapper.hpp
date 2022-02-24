#ifndef TILER_SOLVERS_SAT_UTILS_PBLIB_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_UTILS_PBLIB_WRAPPER_HPP_

#include <memory>
#include <vector>

#include "pb2cnf.h"
#include "solvers/sat_utils/sat_utils.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"

// PBLib wrapper. Takes care of translating between our types and PBLib types when encoding
// pseudo-boolean constraints.
class PBLibWrapper {
public:
    PBLibWrapper(AMO_ENCODER::PB2CNF_AMO_Encoder amo_encoder);

    // Adds clauses that guarantee that at most one of `literals` will be true. Creates new utility
    // variables.
    void at_most_one_of(const sat_utils::Clause& literals,
                        std::unique_ptr<SatWrapper>& sat_wrapper);

private:
    std::unique_ptr<PB2CNF> pb2cnf_;
};

#endif  // TILER_SOLVERS_SAT_UTILS_PBLIB_WRAPPER_HPP_
