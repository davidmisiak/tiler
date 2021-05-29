#ifndef TILER_SOLVERS_SAT_UTILS_CADICAL_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_UTILS_CADICAL_WRAPPER_HPP_

#include <cadical.hpp>
#include <memory>
#include <vector>

#include "solvers/sat_utils/sat_wrapper.hpp"

class CadicalWrapper : public SatWrapper {
public:
    inline void clear() override {
        SatWrapper::clear();
        solver_ = std::make_unique<CaDiCaL::Solver>();
    };

    bool solve() override;
    std::vector<bool> get_model() override;

private:
    std::unique_ptr<CaDiCaL::Solver> solver_ = std::make_unique<CaDiCaL::Solver>();
};

#endif  // TILER_SOLVERS_SAT_UTILS_CADICAL_WRAPPER_HPP_
