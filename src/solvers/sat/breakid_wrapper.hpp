#ifndef TILER_SOLVERS_SAT_BREAKID_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_BREAKID_WRAPPER_HPP_

#include <memory>

#include "breakid.hpp"
#include "solvers/sat/sat_preprocessor.hpp"
#include "solvers/sat/sat_utils.hpp"
#include "solvers/sat/sat_wrapper.hpp"

// BreakID library wrapper for breaking CNF symmetries.
class BreakIDWrapper : public SatPreprocessor {
public:
    virtual void run(std::unique_ptr<SatWrapper>& sat_wrapper, int max_seconds);

    virtual void print_stats();

private:
    BID::BreakID breakid;
};

#endif  // TILER_SOLVERS_SAT_BREAKID_WRAPPER_HPP_
