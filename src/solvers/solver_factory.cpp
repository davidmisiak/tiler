#include "solvers/solver_factory.hpp"

#include <memory>
#include <string>

#include "problem/problem.hpp"
#include "solve_error.hpp"
#include "solvers/sat_solver.hpp"
#include "solvers/simple_solver.hpp"
#include "solvers/solver.hpp"

#ifdef CADICAL
#include "solvers/sat_utils/cadical_wrapper.hpp"
#endif

#ifdef CRYPTOMINISAT
#include "solvers/sat_utils/cryptominisat_wrapper.hpp"
#endif

std::unique_ptr<Solver> solver_factory::create(std::string solver_name, Problem problem) {
    if (solver_name == solver_factory::kSimpleSolver) {
        return std::make_unique<SimpleSolver>(problem);
    }
#ifdef CADICAL
    if (solver_name == solver_factory::kCadicalSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CadicalWrapper>());
    }
#endif
#ifdef CRYPTOMINISAT
    if (solver_name == solver_factory::kCryptominisatSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CryptominisatWrapper>());
    }
#endif
    throw SolveError("Selected solver is not available.");  // this should never happen
}
