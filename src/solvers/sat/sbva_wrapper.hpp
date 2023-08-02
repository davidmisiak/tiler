#ifndef TILER_SOLVERS_SAT_SBVA_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_SBVA_WRAPPER_HPP_

#include <memory>

#include "breakid.hpp"
#include "solvers/sat/sat_preprocessor.hpp"
#include "solvers/sat/sat_utils.hpp"
#include "solvers/sat/sat_wrapper.hpp"

// SBVA library wrapper for preprocessing CNF formulas.
class SbvaWrapper : public SatPreprocessor {
public:
    virtual void run(std::unique_ptr<SatWrapper>& sat_wrapper, int max_seconds);

    virtual void print_stats();

private:
    int added_vars_;
    int removed_clauses_;
};

#endif  // TILER_SOLVERS_SAT_SBVA_WRAPPER_HPP_
