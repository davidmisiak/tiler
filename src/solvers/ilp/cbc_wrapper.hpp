#ifndef TILER_SOLVERS_ILP_CBC_WRAPPER_HPP_
#define TILER_SOLVERS_ILP_CBC_WRAPPER_HPP_

#include <vector>

#include "coin/Cbc_C_Interface.h"
#include "solvers/ilp/ilp_utils.hpp"
#include "solvers/ilp/ilp_wrapper.hpp"

// Wrapper for COIN-OR's CBC ILP solver.
class CbcWrapper : public IlpWrapper {
public:
    // If `adjusted_params` is `true`, we use CBC with some parameter adjustments, otherwise we use
    // the defaults.
    CbcWrapper(bool adjusted_params);

    CbcWrapper(const CbcWrapper&) = delete;
    CbcWrapper(CbcWrapper&&) = default;
    ~CbcWrapper() override;

    bool solve(ilp_utils::ObjectiveSense obj_sense, double obj_limit, bool print_stats,
               int max_seconds) override;

    std::vector<double> get_solution() override;

protected:
    Cbc_Model* model_;
};

#endif  // TILER_SOLVERS_ILP_CBC_WRAPPER_HPP_
