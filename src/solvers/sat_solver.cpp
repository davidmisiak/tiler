#include "solvers/sat_solver.hpp"

#include <memory>
#include <vector>

#include "problem/problem.hpp"
#include "problem/region.hpp"
#include "problem/tile.hpp"
#include "solution/placed_region.hpp"
#include "solution/solution.hpp"
#include "solvers/sat_utils/sat_utils.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"
#include "utils.hpp"

SatSolver::SatSolver(Problem problem, std::unique_ptr<SatWrapper> sat_wrapper)
        : problem_(problem), sat_wrapper_(std::move(sat_wrapper)) {
    int board_size = problem_.board_.get_size();
    for (Tile& tile : problem_.tiles_) {
        tile.limit_count(board_size / tile.get_size());
    }
}

Solution SatSolver::solve() {
    using sat_utils::Lit, sat_utils::Clause;

    int w = problem_.board_.get_width();
    int h = problem_.board_.get_height();
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
                    Lit lit = sat_wrapper_->new_lit();
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
        if (cell_clauses[y][x].size() == 0) continue;
        sat_wrapper_->add_clause(cell_clauses[y][x]);
        at_most_one_of(cell_clauses[y][x]);
    }

    bool result = sat_wrapper_->solve();
    if (!result) return {};

    Solution solution;
    std::vector<bool> model = sat_wrapper_->get_model();
    for (int i = 0; i < static_cast<int>(placed_regions.size()); i++) {
        if (!model[i]) continue;
        solution.push_back(placed_regions[i]);
    }
    return solution;
}

// Uses the commander-variable encoding with k=2 (without special treating of small cases).
// https://www.cs.cmu.edu/~wklieber/papers/2007_efficient-cnf-encoding-for-selecting-1.pdf
void SatSolver::at_most_one_of(sat_utils::Clause literals) {
    while (literals.size() > 1) {
        sat_utils::Clause commanders;
        for (int i = 1; i < static_cast<int>(literals.size()); i += 2) {
            sat_utils::Lit a = literals[i - 1];
            sat_utils::Lit b = literals[i];
            sat_utils::Lit c = sat_wrapper_->new_lit();
            commanders.push_back(c);
            sat_wrapper_->add_clause({~a, ~b});
            sat_wrapper_->add_clause({~a, c});
            sat_wrapper_->add_clause({~b, c});
        }
        if (literals.size() % 2 == 1) {
            commanders.push_back(literals[literals.size() - 1]);
        }
        literals = commanders;
    }
}
