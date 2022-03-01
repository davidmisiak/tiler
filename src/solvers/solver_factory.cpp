#include "solvers/solver_factory.hpp"

#include <memory>
#include <string>

#include "problem/problem.hpp"
#include "solve_error.hpp"
#include "solvers/sat_amo_ordered_solver.hpp"
#include "solvers/sat_amo_solver.hpp"
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
    using namespace AMK_ENCODER;

    if (solver_name == kSimpleSolver) {
        return std::make_unique<SimpleSolver>(problem);
    }

#ifdef CADICAL
    if (solver_name == kCadicalAmoAutoSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CadicalWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BEST, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCadicalAmoNestedSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CadicalWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::NESTED, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCadicalAmoBDDSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CadicalWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BDD, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCadicalAmoBimanderSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CadicalWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BIMANDER, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCadicalAmoCommanderSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CadicalWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::COMMANDER, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCadicalAmoKProductSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CadicalWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::KPRODUCT, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCadicalAmoBinarySolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CadicalWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BINARY, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCadicalAmoPairwiseSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CadicalWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::PAIRWISE, PB2CNF_AMK_Encoder::BEST));
    }

    if (solver_name == kCadicalAmoOrderedSolver) {
        return std::make_unique<SatAmoOrderedSolver>(
                problem, std::make_unique<CadicalWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BDD, PB2CNF_AMK_Encoder::BEST));
    }
#endif

#ifdef CRYPTOMINISAT
    if (solver_name == kCryptominisatAmoAutoSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CryptominisatWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BEST, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCryptominisatAmoNestedSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CryptominisatWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::NESTED, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCryptominisatAmoBDDSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CryptominisatWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BDD, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCryptominisatAmoBimanderSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CryptominisatWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BIMANDER, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCryptominisatAmoCommanderSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CryptominisatWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::COMMANDER, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCryptominisatAmoKProductSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CryptominisatWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::KPRODUCT, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCryptominisatAmoBinarySolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CryptominisatWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BINARY, PB2CNF_AMK_Encoder::BEST));
    }
    if (solver_name == kCryptominisatAmoPairwiseSolver) {
        return std::make_unique<SatAmoSolver>(
                problem, std::make_unique<CryptominisatWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::PAIRWISE, PB2CNF_AMK_Encoder::BEST));
    }

    if (solver_name == kCryptominisatAmoOrderedSolver) {
        return std::make_unique<SatAmoOrderedSolver>(
                problem, std::make_unique<CryptominisatWrapper>(),
                PBLibWrapper(PB2CNF_AMO_Encoder::BDD, PB2CNF_AMK_Encoder::BEST));
    }
#endif

    throw SolveError("Selected solver is not available.");  // this should never happen
}
