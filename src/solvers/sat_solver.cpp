#include "solvers/sat_solver.hpp"

#include <cryptominisat5/cryptominisat.h>

#include <vector>

#include "problem/problem.hpp"
#include "solution/solution.hpp"

SatSolver::SatSolver(Problem problem) : problem_(problem) {}

Solution SatSolver::solve() {
    // example cryptominisat code from their homepage

    CMSat::SATSolver solver;
    std::vector<CMSat::Lit> clause;

    // Let's use 4 threads
    solver.set_num_threads(4);

    // We need 3 variables. They will be: 0,1,2
    // Variable numbers are always trivially increasing
    solver.new_vars(3);

    // add "1 0"
    clause.push_back(CMSat::Lit(0, false));
    solver.add_clause(clause);

    // add "-2 0"
    clause.clear();
    clause.push_back(CMSat::Lit(1, true));
    solver.add_clause(clause);

    // add "-1 2 3 0"
    clause.clear();
    clause.push_back(CMSat::Lit(0, true));
    clause.push_back(CMSat::Lit(1, false));
    clause.push_back(CMSat::Lit(2, false));
    solver.add_clause(clause);

    solver.solve();
    std::cout << "Solution is: " << solver.get_model()[0] << ", " << solver.get_model()[1] << ", "
              << solver.get_model()[2] << std::endl;

    solution_.clear();
    return solution_;
}
