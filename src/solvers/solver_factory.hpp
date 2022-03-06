#ifndef TILER_SOLVERS_SOLVER_FACTORY_HPP_
#define TILER_SOLVERS_SOLVER_FACTORY_HPP_

#include <memory>
#include <string>
#include <vector>

#include "problem/problem.hpp"
#include "solvers/solver.hpp"

// Encapsulates all the ugly #ifdef-s, etc.
namespace solver_factory {

// Returns all available solver names.
std::vector<std::string> get_solver_names();

// Creates a solver instance based on the `solver_name` (which must be one of the strings returned
// by `solver_factory::solver_names()`).
std::unique_ptr<Solver> create(const std::string& solver_name, const Problem& problem);

const std::string kSimpleSolver = "simple";

const std::string kSatPrefix = "sat";

const std::string kCadical = "cadical";
const std::string kCryptominisat = "cms";
const std::vector<std::string> kSatWrapperNames = {
#ifdef CADICAL
        kCadical,
#endif
#ifdef CRYPTOMINISAT
        kCryptominisat,
#endif
};

const std::string kNoSymmetryBreaker = "no-breaker";
const std::string kBreakid = "breakid";
const std::vector<std::string> kSymmetryBreakerNames = {
        kNoSymmetryBreaker,
#ifdef BREAKID
        kBreakid,
#endif
};

const std::string kAmoAuto = "amo-auto";
const std::string kAmoNested = "amo-nested";
const std::string kAmoBDD = "amo-bdd";
const std::string kAmoBimander = "amo-bimander";
const std::string kAmoCommander = "amo-commander";
const std::string kAmoKProduct = "amo-kproduct";
const std::string kAmoBinary = "amo-binary";
const std::string kAmoPairwise = "amo-pairwise";
const std::string kAmoOrdered = "amo-ordered";
const std::string kAmkAuto = "amk-auto";
const std::string kAmkBDD = "amk-bdd";
const std::string kAmkCard = "amk-card";
const std::vector<std::string> kPBLibWrapperNames = {
        kAmoAuto,
        kAmoNested,
        kAmoBDD,
        kAmoBimander,
        kAmoCommander,
        kAmoKProduct,
        kAmoBinary,
        // kAmoPairwise,  // crashes on larger problems because of RAM exhaustion
        kAmoOrdered,
        kAmkAuto,
        kAmkBDD,
        kAmkCard,
};

}  // namespace solver_factory

#endif  // TILER_SOLVERS_SOLVER_FACTORY_HPP_
