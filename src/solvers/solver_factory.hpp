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
std::unique_ptr<Solver> create(std::string solver_name, Problem problem);

const std::string kSimpleSolver = "simple";
const std::string kCadicalSolver = "cadical";
const std::string kCryptominisatSolver = "cmsat";

// Contains all available solver names.
// We don't use an enum for solver names because CLI11's error messages for enums are somewhat ugly
// and enum -> vector conversion would be an unnecessary hassle.
const std::vector<std::string> solver_names = {
        kSimpleSolver,
#ifdef CADICAL
        kCadicalSolver,
#endif
#ifdef CRYPTOMINISAT
        kCryptominisatSolver,
#endif
};

}  // namespace solver_factory

#endif  // TILER_SOLVERS_SOLVER_FACTORY_HPP_
