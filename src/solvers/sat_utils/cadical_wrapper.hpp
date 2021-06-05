#ifndef TILER_SOLVERS_SAT_UTILS_CADICAL_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_UTILS_CADICAL_WRAPPER_HPP_

#include <vector>

#include "cadical.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"

class CadicalWrapper : public SatWrapper {
public:
    bool solve() override;
    std::vector<bool> get_model() override;

private:
    CaDiCaL::Solver solver_;
};

#endif  // TILER_SOLVERS_SAT_UTILS_CADICAL_WRAPPER_HPP_
