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

// simple
const std::string kSimplePrefix = "simple";

const std::string kSimpleDefault = "default";
const std::string kSimpleFrequentFirst = "frequent";
const std::string kSimpleRareFirst = "rare";
const std::string kSimpleFillTopRow = "fill-row";
const std::vector<std::string> kSimpleOrderingNames = {
        kSimpleDefault,
        kSimpleFrequentFirst,
        kSimpleRareFirst,
        kSimpleFillTopRow,
};

// dlx
const std::string kDlxSolver = "dlx";

// sat
const std::string kSatPrefix = "sat";

const std::string kSatCadical = "cadical";
const std::string kSatCryptominisat = "cms";
const std::string kSatKissat = "kissat";
const std::vector<std::string> kSatWrapperNames = {
#ifdef CADICAL
        kSatCadical,
#endif
#ifdef CRYPTOMINISAT
        kSatCryptominisat,
#endif
#ifdef KISSAT
        kSatKissat,
#endif
};

const std::string kSatNoSymmetryBreaker = "no-breaker";
const std::string kSatBreakid = "breakid";
const std::vector<std::string> kSatSymmetryBreakerNames = {
        kSatNoSymmetryBreaker,
#ifdef BREAKID
        kSatBreakid,
#endif
};

const std::string kSatAmoAuto = "amo-auto";
const std::string kSatAmoNested = "amo-nested";
const std::string kSatAmoBDD = "amo-seq";
const std::string kSatAmoBimander = "amo-bimander";
const std::string kSatAmoCommander = "amo-commander";
const std::string kSatAmoKProduct = "amo-kproduct";
const std::string kSatAmoBinary = "amo-binary";
const std::string kSatAmoPairwise = "amo-pairwise";
const std::string kSatAmoOrdered = "amo-ordered";
const std::string kSatAmkAuto = "amk-auto";
const std::string kSatAmkBDD = "amk-bdd";
const std::string kSatAmkCard = "amk-card";
const std::vector<std::string> kSatPBLibWrapperNames = {
        kSatAmoAuto,
        // kSatAmoNested,  // crashes on larger problems because of RAM exhaustion (with breakid)
        kSatAmoBDD,
        kSatAmoBimander,
        kSatAmoCommander,
        kSatAmoKProduct,
        kSatAmoBinary,
        // kSatAmoPairwise,  // crashes on larger problems because of RAM exhaustion
        kSatAmoOrdered,
        kSatAmkAuto,
        kSatAmkBDD,
        kSatAmkCard,
};

// ilp
const std::string kIlpPrefix = "ilp";

const std::string kIlpCbc = "cbc";
const std::string kIlpGurobi = "gurobi";
const std::vector<std::string> kIlpWrapperNames = {
        kIlpCbc,
#ifdef GUROBI
        kIlpGurobi,
#endif
};

const std::string kIlpDefaultParams = "default";
const std::string kIlpAdjustedParams = "adjusted";
const std::vector<std::string> kIlpParamsNames = {
        kIlpDefaultParams,
        kIlpAdjustedParams,
};

const std::string kIlpEqIgnore = "eq-ign";
const std::string kIlpEqMinimize = "eq-min";
const std::string kIlpGeqMinimize = "geq-min";
const std::string kIlpLeqMaximize = "leq-max";
const std::vector<std::string> kIlpObjectiveNames = {
        kIlpEqIgnore,
        kIlpEqMinimize,
        kIlpGeqMinimize,
        kIlpLeqMaximize,
};

// csp
const std::string kCspPrefix = "csp";

const std::string kCspGecode = "gecode";
const std::string kCspChuffed = "chuffed";
const std::string kCspGurobi = "gurobi";
const std::vector<std::string> kCspFlatSolverNames = {
#ifdef MINIZINC
        kCspGecode,
#ifdef CHUFFED
        kCspChuffed,
#endif
#ifdef GUROBI
        kCspGurobi,
#endif
#endif  // MINIZINC
};

}  // namespace solver_factory

#endif  // TILER_SOLVERS_SOLVER_FACTORY_HPP_
