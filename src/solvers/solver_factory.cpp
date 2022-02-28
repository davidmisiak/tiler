#include "solvers/solver_factory.hpp"

#include <memory>
#include <string>

#include "problem/problem.hpp"
#include "solve_error.hpp"
#include "solvers/sat_oti_solver.hpp"
#include "solvers/sat_solver.hpp"
#include "solvers/simple_solver.hpp"
#include "solvers/solver.hpp"

#ifdef CADICAL
#include "solvers/sat_utils/cadical_wrapper.hpp"
#endif

#ifdef CRYPTOMINISAT
#include "solvers/sat_utils/cryptominisat_wrapper.hpp"
#endif

#if defined(CADICAL) || defined(CRYPTOMINISAT)
#include "pb2cnf.h"
#include "solvers/sat_utils/pblib_wrapper.hpp"
#endif

std::unique_ptr<Solver> solver_factory::create(const std::string& solver_name,
                                               const Problem& problem) {
    using namespace solver_factory;
    using namespace AMO_ENCODER;

    if (solver_name == kSimpleSolver) {
        return std::make_unique<SimpleSolver>(problem);
    }

#ifdef CADICAL
    if (solver_name == kCadicalAutoSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CadicalWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::BEST));
    }
    if (solver_name == kCadicalNestedSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CadicalWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::NESTED));
    }
    if (solver_name == kCadicalBDDSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CadicalWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::BDD));
    }
    if (solver_name == kCadicalBimanderSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CadicalWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::BIMANDER));
    }
    if (solver_name == kCadicalCommanderSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CadicalWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::COMMANDER));
    }
    if (solver_name == kCadicalKProductSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CadicalWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::KPRODUCT));
    }
    if (solver_name == kCadicalBinarySolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CadicalWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::BINARY));
    }
    if (solver_name == kCadicalPairwiseSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CadicalWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::PAIRWISE));
    }

    if (solver_name == kCadicalOTISolver) {
        return std::make_unique<SatOTISolver>(problem, std::make_unique<CadicalWrapper>(),
                                              PBLibWrapper(PB2CNF_AMO_Encoder::BDD));
    }
#endif

#ifdef CRYPTOMINISAT
    if (solver_name == kCryptominisatAutoSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CryptominisatWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::BEST));
    }
    if (solver_name == kCryptominisatNestedSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CryptominisatWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::NESTED));
    }
    if (solver_name == kCryptominisatBDDSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CryptominisatWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::BDD));
    }
    if (solver_name == kCryptominisatBimanderSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CryptominisatWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::BIMANDER));
    }
    if (solver_name == kCryptominisatCommanderSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CryptominisatWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::COMMANDER));
    }
    if (solver_name == kCryptominisatKProductSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CryptominisatWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::KPRODUCT));
    }
    if (solver_name == kCryptominisatBinarySolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CryptominisatWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::BINARY));
    }
    if (solver_name == kCryptominisatPairwiseSolver) {
        return std::make_unique<SatSolver>(problem, std::make_unique<CryptominisatWrapper>(),
                                           PBLibWrapper(PB2CNF_AMO_Encoder::PAIRWISE));
    }

    if (solver_name == kCryptominisatOTISolver) {
        return std::make_unique<SatOTISolver>(problem, std::make_unique<CryptominisatWrapper>(),
                                              PBLibWrapper(PB2CNF_AMO_Encoder::BDD));
    }
#endif

    throw SolveError("Selected solver is not available.");  // this should never happen
}
