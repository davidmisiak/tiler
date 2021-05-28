#include "solvers/sat_solver.hpp"

#include <cryptominisat5/cryptominisat.h>

#include <vector>

#include "problem/problem.hpp"
#include "problem/region.hpp"
#include "problem/tile.hpp"
#include "solution/placed_region.hpp"
#include "solution/solution.hpp"
#include "utils.hpp"

SatSolver::SatSolver(Problem problem) : problem_(problem) {
    int board_size = problem_.board_.get_size();
    for (Tile& tile : problem_.tiles_) {
        tile.limit_count(board_size / tile.get_size());
    }
}

Solution SatSolver::solve() {
    next_var_ = 0;
    clauses_.clear();

    int w = problem_.board_.get_width();
    int h = problem_.board_.get_height();
    utils::BoolMatrix matrix = problem_.board_.get_matrix();

    std::vector<Clause> tile_clauses;
    std::vector<std::vector<Clause>> cell_clauses(h, std::vector<Clause>(w, Clause{}));
    std::vector<PlacedRegion> placed_regions;
    for (Tile tile : problem_.tiles_) {
        for (int i = 0; i < tile.get_count(); i++) {
            Clause tile_clause;
            for (auto [bx, by] : problem_.board_.get_cells()) {
                for (Region region : tile) {
                    int sx = bx - region.get_top_left_x();
                    int sy = by - region.get_top_left_y();
                    if (!problem_.board_.has_subregion(sx, sy, region)) continue;
                    placed_regions.push_back({sx, sy, region});
                    CMSat::Lit lit(next_var_++, false);
                    tile_clause.push_back(lit);
                    for (auto [rx, ry] : region.get_cells()) {
                        cell_clauses[sy + ry][sx + rx].push_back(lit);
                    }
                }
            }
            tile_clauses.push_back(tile_clause);
        }
    }

    for (Clause tile_clause : tile_clauses) {
        at_most_one_of(tile_clause);
    }
    for (auto [x, y] : problem_.board_.get_cells()) {
        clauses_.push_back(cell_clauses[y][x]);
        at_most_one_of(cell_clauses[y][x]);
    }

    CMSat::SATSolver solver;
    solver.new_vars(next_var_);
    for (Clause clause : clauses_) {
        solver.add_clause(clause);
    }

    CMSat::lbool result = solver.solve();
    Solution solution;
    if (result == CMSat::l_False) return solution;

    std::vector<CMSat::lbool> model = solver.get_model();
    for (unsigned int i = 0; i < placed_regions.size(); i++) {
        if (model[i] == CMSat::l_False) continue;
        solution.push_back(placed_regions[i]);
    }
    return solution;
}

void SatSolver::at_most_one_of(SatSolver::Clause literals) {
    std::vector<CMSat::Lit> commanders;
    for (unsigned int i = 1; i < literals.size(); i += 2) {
        CMSat::Lit a = literals[i - 1];
        CMSat::Lit b = literals[i];
        CMSat::Lit c(next_var_++, false);
        commanders.push_back(c);
        clauses_.push_back({~a, ~b});
        clauses_.push_back({~a, c});
        clauses_.push_back({~b, c});
    }
    if (literals.size() % 2 == 1) {
        commanders.push_back(literals[literals.size() - 1]);
    }
    if (commanders.size() > 1) {
        at_most_one_of(commanders);
    }
}
