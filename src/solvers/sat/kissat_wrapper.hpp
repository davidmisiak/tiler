#ifndef TILER_SOLVERS_SAT_KISSAT_WRAPPER_HPP_
#define TILER_SOLVERS_SAT_KISSAT_WRAPPER_HPP_

#include <vector>

#include "solvers/sat/sat_wrapper.hpp"

extern "C" {
typedef struct kissat kissat;
kissat* kissat_init(void);
void kissat_set_option(kissat*, const char*, int);
void kissat_add(kissat*, int);
int kissat_solve(kissat*);
int kissat_value(kissat*, int);
void kissat_terminate(kissat*);
void kissat_release(kissat*);
}

class KissatWrapper : public SatWrapper {
public:
    KissatWrapper();

    KissatWrapper(const KissatWrapper&) = delete;
    KissatWrapper(KissatWrapper&&) = default;
    ~KissatWrapper() override;

    bool solve(int max_seconds) override;
    std::vector<bool> get_model() override;

private:
    kissat* solver_;
};

#endif  // TILER_SOLVERS_SAT_KISSAT_WRAPPER_HPP_
