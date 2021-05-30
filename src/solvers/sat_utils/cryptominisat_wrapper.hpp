#ifndef TILER_SOLVERS_SAT_UTILS_CRYPTOMINISAT_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_UTILS_CRYPTOMINISAT_WRAPPER_HPP_

#include <memory>
#include <vector>

#include "cryptominisat5/cryptominisat.h"
#include "solvers/sat_utils/sat_wrapper.hpp"

class CryptominisatWrapper : public SatWrapper {
public:
    inline void clear() override {
        SatWrapper::clear();
        solver_ = std::make_unique<CMSat::SATSolver>();
    };

    bool solve() override;
    std::vector<bool> get_model() override;

private:
    std::unique_ptr<CMSat::SATSolver> solver_ = std::make_unique<CMSat::SATSolver>();
};

#endif  // TILER_SOLVERS_SAT_UTILS_CRYPTOMINISAT_WRAPPER_HPP_
