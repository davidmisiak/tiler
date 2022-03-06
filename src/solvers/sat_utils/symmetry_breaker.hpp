#ifndef TILER_SOLVERS_SAT_UTILS_SYMMETRY_BREAKER_HPP_
#define TILER_SOLVERS_SAT_UTILS_SYMMETRY_BREAKER_HPP_

#include <memory>

#include "solvers/sat_utils/sat_utils.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"

// Interaface for CNF symmetry breaking provider. This class can be instantiated as a 'dummy'
// symmetry breaker that does nothing.
class SymmetryBreaker {
public:
    virtual ~SymmetryBreaker() = default;

    virtual void break_symmetries([[maybe_unused]] std::unique_ptr<SatWrapper>& sat_wrapper){};

    virtual void print_stats(){};
};

#endif  // TILER_SOLVERS_SAT_UTILS_SYMMETRY_BREAKER_HPP_
