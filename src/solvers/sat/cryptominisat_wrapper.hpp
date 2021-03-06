#ifndef TILER_SOLVERS_SAT_CRYPTOMINISAT_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_CRYPTOMINISAT_WRAPPER_HPP_

#include <vector>

#include "cryptominisat5/cryptominisat.h"
#include "solvers/sat/sat_wrapper.hpp"

class CryptominisatWrapper : public SatWrapper {
public:
    bool solve(int max_seconds) override;
    std::vector<bool> get_model() override;

private:
    CMSat::SATSolver solver_;
};

#endif  // TILER_SOLVERS_SAT_CRYPTOMINISAT_WRAPPER_HPP_
