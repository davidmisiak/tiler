#include "solvers/sat_amo_ordered_solver.hpp"

#include <memory>
#include <vector>

#include "print.hpp"
#include "problem/problem.hpp"
#include "problem/region.hpp"
#include "problem/tile.hpp"
#include "solution/placed_region.hpp"
#include "solution/solution.hpp"
#include "solvers/sat_utils/sat_utils.hpp"
#include "solvers/sat_utils/sat_wrapper.hpp"
#include "utils.hpp"

SatAmoOrderedSolver::SatAmoOrderedSolver(Problem problem, std::unique_ptr<SatWrapper> sat_wrapper,
                                         PBLibWrapper pblib_wrapper)
        : problem_(problem),
          sat_wrapper_(std::move(sat_wrapper)),
          pblib_wrapper_(std::move(pblib_wrapper)) {
    int board_size = problem_.board_.get_size();
    for (Tile& tile : problem_.tiles_) {
        tile.limit_count(board_size / tile.get_size());
    }
}

// The principle is the same as in SatSolver. However, we define a certain order in which tile
// instances of a specific tile can be placed on the board (ascending positions returned by
// `board_.get_cells()`). Thanks to this, we can omit some tile instance positions (e.g. the second
// instance cannot occupy the first possible position, because the first instance would have no
// position available) - this is accomplished by `position_number`. Furthemore, we add clauses that
// ensure that tile instance positions are ordered. We exploit the auxiliary variables added by
// sequential (BDD) encoding of the "each tile instance has at most one position" constraints - we
// add analogous clauses in the orthogonal direction to imply the position ordering.
Solution SatAmoOrderedSolver::solve(bool print_stats) {
    using sat_utils::Lit, sat_utils::Clause;

    int w = problem_.board_.get_width();
    int h = problem_.board_.get_height();
    std::vector<std::vector<Clause>> tile_clauses;
    std::vector<std::vector<Clause>> cell_clauses(h, std::vector<Clause>(w, Clause{}));
    std::vector<PlacedRegion> placed_regions;
    for (const Tile& tile : problem_.tiles_) {
        std::vector<Clause> instance_clauses;
        for (int i = 0; i < tile.get_count(); i++) {
            Clause instance_clause;
            int position_number = 0;
            for (auto [bx, by] : problem_.board_.get_cells()) {
                for (const Region& region : tile) {
                    int sx = bx - region.get_top_left_x();
                    int sy = by - region.get_top_left_y();
                    if (!problem_.board_.has_subregion(sx, sy, region)) continue;

                    position_number++;
                    if (position_number <= i) continue;

                    placed_regions.push_back({sx, sy, region});
                    Lit lit = sat_wrapper_->new_lit();
                    instance_clause.push_back(lit);
                    for (auto [rx, ry] : region.get_cells()) {
                        cell_clauses[sy + ry][sx + rx].push_back(lit);
                    }
                }
            }
            instance_clauses.push_back(instance_clause);
        }
        tile_clauses.push_back(instance_clauses);
    }

    for (const std::vector<Clause>& instance_clauses : tile_clauses) {
        // create aux variables
        std::vector<Clause> aux_clauses;
        for (const Clause& instance_clause : instance_clauses) {
            Clause aux_clause;
            for (int v = 0; v < static_cast<int>(instance_clause.size()) - 1; v++) {
                aux_clause.push_back(sat_wrapper_->new_lit());
            }
            aux_clauses.push_back(aux_clause);
        }

        // add clauses
        for (int c = 0; c < static_cast<int>(instance_clauses.size()); c++) {
            // each tile instance has at most one position
            //  similar to external/pblib/pblib/encoder/BDD_Seq_Amo.cpp
            if (instance_clauses[c].size() < 2) continue;
            sat_wrapper_->add_clause({~instance_clauses[c][0], aux_clauses[c][0]});
            sat_wrapper_->add_clause({~aux_clauses[c][0], ~instance_clauses[c][1]});
            for (int v = 1; v < static_cast<int>(instance_clauses[c].size()) - 1; v++) {
                sat_wrapper_->add_clause({~instance_clauses[c][v], aux_clauses[c][v]});
                sat_wrapper_->add_clause({~aux_clauses[c][v - 1], aux_clauses[c][v]});
                sat_wrapper_->add_clause({~aux_clauses[c][v], ~instance_clauses[c][v + 1]});
            }
            // tile instances are ordered by their position
            if (c > 0) {
                for (int v = 0; v < static_cast<int>(instance_clauses[c].size()) - 1; v++) {
                    sat_wrapper_->add_clause({~aux_clauses[c][v], aux_clauses[c - 1][v]});
                }
            }
        }
    }

    for (auto [x, y] : problem_.board_.get_cells()) {
        if (cell_clauses[y][x].size() == 0) {
            // the cell cannot be covered, the problem is unsolvable
            return {};
        }
        pblib_wrapper_.exactly_k(cell_clauses[y][x], sat_wrapper_, 1);
    }

    if (print_stats) {
        print::stats() << placed_regions.size() << " placed regions\n"
                       << sat_wrapper_->get_var_count() << " boolean variables\n"
                       << sat_wrapper_->get_clause_count() << " boolean clauses\n"
                       << sat_wrapper_->get_lit_count() << " boolean literals\n";
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
