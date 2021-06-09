#ifndef TILER_SOLVERS_SAT_UTILS_CRYPTOMINISAT_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_UTILS_CRYPTOMINISAT_WRAPPER_HPP_

#include <vector>

#include "cryptominisat5/cryptominisat.h"
#include "solvers/sat_utils/sat_wrapper.hpp"

class CryptominisatWrapper : public SatWrapper {
public:
    bool solve() override;
    std::vector<bool> get_model() override;

private:
    CMSat::SATSolver solver_;
};

#endif  // TILER_SOLVERS_SAT_UTILS_CRYPTOMINISAT_WRAPPER_HPP_
