#ifndef TILER_SOLVERS_SAT_UTILS_CADICAL_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_UTILS_CADICAL_WRAPPER_HPP_

#include <vector>

#include "cadical.hpp"
#include "signal.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"

class CadicalWrapper : public SatWrapper, public CaDiCaL::Handler {
public:
    // CaDiCaL::Handler methods
    void catch_signal([[maybe_unused]] int sig) override{};
    void catch_alarm() override { solver_.terminate(); };

    bool solve(int max_seconds) override;
    std::vector<bool> get_model() override;

private:
    CaDiCaL::Solver solver_;
};

#endif  // TILER_SOLVERS_SAT_UTILS_CADICAL_WRAPPER_HPP_
