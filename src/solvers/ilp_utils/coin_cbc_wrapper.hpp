#ifndef TILER_SOLVERS_ILP_UTILS_COIN_CBC_WRAPPER_HPP_
#define TILER_SOLVERS_ILP_UTILS_COIN_CBC_WRAPPER_HPP_

#include <vector>

#include "coin/Cbc_C_Interface.h"
#include "solvers/ilp_utils/ilp_utils.hpp"
#include "solvers/ilp_utils/ilp_wrapper.hpp"

// Wrapper for COIN-CBC ILP solver.
class CoinCbcWrapper : public IlpWrapper {
public:
    // If `adjusted_params` is `true`, we use COIN-CBC with some parameter adjustments, otherwise we
    // use the defaults.
    CoinCbcWrapper(bool adjusted_params);

    CoinCbcWrapper(const CoinCbcWrapper&) = delete;
    CoinCbcWrapper(CoinCbcWrapper&&) = default;
    ~CoinCbcWrapper() override;

    bool solve(ilp_utils::ObjectiveSense obj_sense, double obj_limit, bool print_stats,
               int max_seconds) override;

    std::vector<double> get_solution() override;

protected:
    Cbc_Model* model_;
};

#endif  // TILER_SOLVERS_ILP_UTILS_COIN_CBC_WRAPPER_HPP_
