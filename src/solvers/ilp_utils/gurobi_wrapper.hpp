#ifndef TILER_SOLVERS_ILP_UTILS_GUROBI_WRAPPER_HPP_
#define TILER_SOLVERS_ILP_UTILS_GUROBI_WRAPPER_HPP_

#include <memory>
#include <vector>

#include "gurobi_c++.h"
#include "solvers/ilp_utils/ilp_utils.hpp"
#include "solvers/ilp_utils/ilp_wrapper.hpp"

// Wrapper for Gurobi ILP solver.
class GurobiWrapper : public IlpWrapper {
public:
    // If `adjusted_params` is `true`, we use Gurobi with some parameter adjustments, otherwise we
    // use the defaults.
    GurobiWrapper(bool adjusted_params);

    bool solve(ilp_utils::ObjectiveSense obj_sense, double obj_limit, bool print_stats,
               int max_seconds) override;

    std::vector<double> get_solution() override;

protected:
    std::unique_ptr<GRBModel> model_;
    std::vector<GRBVar> gurobi_vars_;
    bool adjusted_params_;
};

#endif  // TILER_SOLVERS_ILP_UTILS_GUROBI_WRAPPER_HPP_
