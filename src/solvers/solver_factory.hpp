#ifndef TILER_SOLVERS_SOLVER_FACTORY_HPP_
#define TILER_SOLVERS_SOLVER_FACTORY_HPP_

#include <memory>
#include <string>

#include "problem/problem.hpp"
#include "solvers/solver.hpp"

// Encapsulates all the ugly #ifdef-s, etc.
namespace solver_factory {

// Creates a solver instance based on the `solver_name` (which should be one of the strings in
// `solver_factory::solver_names`).
std::unique_ptr<Solver> create(const std::string& solver_name, const Problem& problem);

const std::string kSimpleSolver = "simple";

const std::string kCadicalAmoAutoSolver = "cadical-amo-auto";
const std::string kCadicalAmoNestedSolver = "cadical-amo-nested";
const std::string kCadicalAmoBDDSolver = "cadical-amo-bdd";
const std::string kCadicalAmoBimanderSolver = "cadical-amo-bimander";
const std::string kCadicalAmoCommanderSolver = "cadical-amo-commander";
const std::string kCadicalAmoKProductSolver = "cadical-amo-kproduct";
const std::string kCadicalAmoBinarySolver = "cadical-amo-binary";
const std::string kCadicalAmoPairwiseSolver = "cadical-amo-pairwise";

const std::string kCadicalAmoOrderedSolver = "cadical-amo-ordered";

const std::string kCryptominisatAmoAutoSolver = "cmsat-amo-auto";
const std::string kCryptominisatAmoNestedSolver = "cmsat-amo-nested";
const std::string kCryptominisatAmoBDDSolver = "cmsat-amo-bdd";
const std::string kCryptominisatAmoBimanderSolver = "cmsat-amo-bimander";
const std::string kCryptominisatAmoCommanderSolver = "cmsat-amo-commander";
const std::string kCryptominisatAmoKProductSolver = "cmsat-amo-kproduct";
const std::string kCryptominisatAmoBinarySolver = "cmsat-amo-binary";
const std::string kCryptominisatAmoPairwiseSolver = "cmsat-amo-pairwise";

const std::string kCryptominisatAmoOrderedSolver = "cmsat-amo-ordered";

// Contains all available solver names.
// We don't use an enum for solver names because CLI11's error messages for enums are somewhat ugly
// and enum -> vector conversion would be an unnecessary hassle.
const std::vector<std::string> solver_names = {
        kSimpleSolver,
#ifdef CADICAL
        kCadicalAmoAutoSolver,
        kCadicalAmoNestedSolver,
        kCadicalAmoBDDSolver,
        kCadicalAmoBimanderSolver,
        kCadicalAmoCommanderSolver,
        kCadicalAmoKProductSolver,
        kCadicalAmoBinarySolver,
        // kCadicalPairwiseSolver,  // crashes on larger problems because of RAM exhaustion
        kCadicalAmoOrderedSolver,
#endif
#ifdef CRYPTOMINISAT
        kCryptominisatAmoAutoSolver,
        kCryptominisatAmoNestedSolver,
        kCryptominisatAmoBDDSolver,
        kCryptominisatAmoBimanderSolver,
        kCryptominisatAmoCommanderSolver,
        kCryptominisatAmoKProductSolver,
        kCryptominisatAmoBinarySolver,
        // kCryptominisatPairwiseSolver,  // crashes on larger problems because of RAM exhaustion
        kCryptominisatAmoOrderedSolver,
#endif
};

}  // namespace solver_factory

#endif  // TILER_SOLVERS_SOLVER_FACTORY_HPP_
