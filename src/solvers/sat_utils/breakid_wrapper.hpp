#ifndef TILER_SOLVERS_SAT_UTILS_BREAKID_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_UTILS_BREAKID_WRAPPER_HPP_

#include <memory>

#include "breakid/breakid.hpp"
#include "solvers/sat_utils/sat_utils.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"
#include "solvers/sat_utils/symmetry_breaker.hpp"

// BreakID library wrapper for breaking CNF symmetries.
class BreakIDWrapper : public SymmetryBreaker {
public:
    virtual void break_symmetries(std::unique_ptr<SatWrapper>& sat_wrapper);

    virtual void print_stats();

private:
    BID::BreakID breakid;
};

#endif  // TILER_SOLVERS_SAT_UTILS_BREAKID_WRAPPER_HPP_
