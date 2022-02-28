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

const std::string kCadicalAutoSolver = "cadical-auto";
const std::string kCadicalNestedSolver = "cadical-nested";
const std::string kCadicalBDDSolver = "cadical-bdd";
const std::string kCadicalBimanderSolver = "cadical-bimander";
const std::string kCadicalCommanderSolver = "cadical-commander";
const std::string kCadicalKProductSolver = "cadical-kproduct";
const std::string kCadicalBinarySolver = "cadical-binary";
const std::string kCadicalPairwiseSolver = "cadical-pairwise";

const std::string kCadicalOTISolver = "cadical-oti";

const std::string kCryptominisatAutoSolver = "cmsat-auto";
const std::string kCryptominisatNestedSolver = "cmsat-nested";
const std::string kCryptominisatBDDSolver = "cmsat-bdd";
const std::string kCryptominisatBimanderSolver = "cmsat-bimander";
const std::string kCryptominisatCommanderSolver = "cmsat-commander";
const std::string kCryptominisatKProductSolver = "cmsat-kproduct";
const std::string kCryptominisatBinarySolver = "cmsat-binary";
const std::string kCryptominisatPairwiseSolver = "cmsat-pairwise";

const std::string kCryptominisatOTISolver = "cmsat-oti";

// Contains all available solver names.
// We don't use an enum for solver names because CLI11's error messages for enums are somewhat ugly
// and enum -> vector conversion would be an unnecessary hassle.
const std::vector<std::string> solver_names = {
        kSimpleSolver,
#ifdef CADICAL
        kCadicalAutoSolver,
        kCadicalNestedSolver,
        kCadicalBDDSolver,
        kCadicalBimanderSolver,
        kCadicalCommanderSolver,
        kCadicalKProductSolver,
        kCadicalBinarySolver,
        kCadicalPairwiseSolver,
        kCadicalOTISolver,
#endif
#ifdef CRYPTOMINISAT
        kCryptominisatAutoSolver,
        kCryptominisatNestedSolver,
        kCryptominisatBDDSolver,
        kCryptominisatBimanderSolver,
        kCryptominisatCommanderSolver,
        kCryptominisatKProductSolver,
        kCryptominisatBinarySolver,
        kCryptominisatPairwiseSolver,
        kCryptominisatOTISolver,
#endif
};

}  // namespace solver_factory

#endif  // TILER_SOLVERS_SOLVER_FACTORY_HPP_
