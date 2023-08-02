#ifndef TILER_SOLVERS_SAT_SAT_PREPROCESSOR_HPP_
#define TILER_SOLVERS_SAT_SAT_PREPROCESSOR_HPP_

#include <memory>

#include "solvers/sat/sat_utils.hpp"
#include "solvers/sat/sat_wrapper.hpp"

// Interaface for CNF preprocessing provider. This class can be instantiated as a 'dummy'
// preprocessor that does nothing.
class SatPreprocessor {
public:
    virtual ~SatPreprocessor() = default;

    virtual void run([[maybe_unused]] std::unique_ptr<SatWrapper>& sat_wrapper,
                     [[maybe_unused]] int max_seconds){};

    virtual void print_stats(){};
};

#endif  // TILER_SOLVERS_SAT_SAT_PREPROCESSOR_HPP_
