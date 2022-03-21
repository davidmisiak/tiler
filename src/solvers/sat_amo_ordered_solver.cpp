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
#include "solvers/sat_utils/symmetry_breaker.hpp"
#include "utils.hpp"

SatAmoOrderedSolver::SatAmoOrderedSolver(Problem problem, std::unique_ptr<SatWrapper> sat_wrapper,
                                         std::unique_ptr<SymmetryBreaker> symmetry_breaker,
                                         PBLibWrapper pblib_wrapper)
        : problem_(problem),
          sat_wrapper_(std::move(sat_wrapper)),
          symmetry_breaker_(std::move(symmetry_breaker)),
          pblib_wrapper_(std::move(pblib_wrapper)) {
    problem_.limit_tile_counts();
}

// The principle is the same as in SatSolver. However, we define a certain order in which tile
// instances of a specific tile can be placed on the board (ascending positions returned by
// `board_.get_cells()`). Thanks to this, we can omit some tile instance positions (e.g. the second
// instance cannot occupy the first possible position, because the first instance would have no
// position available) - this is accomplished by `position_number`. Furthemore, we add clauses that
// ensure that tile instance positions are ordered. We exploit the auxiliary variables added by
// sequential (BDD) encoding of the "each tile instance has at most one position" constraints - we
// add analogous clauses in the orthogonal direction to imply the position ordering.
Solution SatAmoOrderedSolver::solve(bool print_stats, int max_seconds) {
    using sat_utils::Lit, sat_utils::Clause;

    int w = problem_.board_.get_width();
    int h = problem_.board_.get_height();
    bool exact_tile_set = problem_.extra_tile_square_count() == 0;
    std::vector<std::vector<Clause>> tile_clauses;
    std::vector<std::vector<Clause>> cell_clauses(h, std::vector<Clause>(w, Clause{}));
    std::vector<PlacedRegion> placed_regions;

    auto board_cells = problem_.board_.get_cells();
    for (const Tile& tile : problem_.tiles_) {
        int tile_count = tile.get_count();
        int add_instance_clause = true;
        if (tile_count * tile.get_size() >= problem_.board_.get_size()) {
            // If there are enough instances of the single tile to cover the board, we can drop the
            // entire notion of tile instances and just encode cell covering.
            tile_count = 1;
            add_instance_clause = false;
        }

        std::vector<Clause> instance_clauses;
        for (int i = 0; i < tile_count; i++) {
            Clause instance_clause;
            int position_number = 0;
            for (auto [bx, by] : board_cells) {
                for (const Region& region : tile) {
                    int sx = bx - region.get_top_left_x();
                    int sy = by - region.get_top_left_y();
                    if (!problem_.board_.has_subregion(sx, sy, region)) continue;

                    position_number++;
                    if (position_number <= i) {
                        // Minor optimization - `i`-th tile can be placed only on the `i`-th
                        // position or later.
                        continue;
                    }

                    placed_regions.push_back({sx, sy, region});
                    Lit lit = sat_wrapper_->new_lit();
                    instance_clause.push_back(lit);
                    for (auto [rx, ry] : region.get_cells()) {
                        cell_clauses[sy + ry][sx + rx].push_back(lit);
                    }
                }
            }
            if (add_instance_clause) {
                instance_clauses.push_back(instance_clause);
            }
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
            if (instance_clauses[c].size() == 0) continue;
            if (instance_clauses[c].size() == 1) {
                if (exact_tile_set) {
                    // We know that each tile instance has to be used, so we force the variable to
                    // be true.
                    sat_wrapper_->add_clause({instance_clauses[c][0]});
                }
                continue;
            }
            if (exact_tile_set) {
                // We know that each tile instance has to be used, so we enforce that at least one
                // of the variables is true.
                sat_wrapper_->add_clause({instance_clauses[c].back(), aux_clauses[c].back()});
            }

            // Each tile instance has at most one position.
            // (similar to external/pblib/pblib/encoder/BDD_Seq_Amo.cpp)
            sat_wrapper_->add_clause({~instance_clauses[c][0], aux_clauses[c][0]});
            sat_wrapper_->add_clause({~aux_clauses[c][0], ~instance_clauses[c][1]});
            for (int v = 1; v < static_cast<int>(instance_clauses[c].size()) - 1; v++) {
                sat_wrapper_->add_clause({~instance_clauses[c][v], aux_clauses[c][v]});
                sat_wrapper_->add_clause({~aux_clauses[c][v - 1], aux_clauses[c][v]});
                sat_wrapper_->add_clause({~aux_clauses[c][v], ~instance_clauses[c][v + 1]});
            }
            // Tile instances are ordered by their position.
            if (c > 0) {
                for (int v = 0; v < static_cast<int>(instance_clauses[c].size()) - 1; v++) {
                    sat_wrapper_->add_clause({~aux_clauses[c][v], aux_clauses[c - 1][v]});
                }
            }
        }
    }

    for (auto [x, y] : board_cells) {
        if (cell_clauses[y][x].size() == 0) {
            // the cell cannot be covered, the problem is unsolvable
            return {};
        }
        pblib_wrapper_.exactly_k(cell_clauses[y][x], sat_wrapper_, 1);
    }

    symmetry_breaker_->break_symmetries(sat_wrapper_);

    if (print_stats) {
        print::stats() << placed_regions.size() << " placed regions\n";
        sat_wrapper_->print_stats();
        symmetry_breaker_->print_stats();
    }

    bool result = sat_wrapper_->solve(max_seconds);
    if (!result) return {};

    Solution solution;
    std::vector<bool> model = sat_wrapper_->get_model();
    for (int i = 0; i < static_cast<int>(placed_regions.size()); i++) {
        if (!model[i]) continue;
        solution.push_back(placed_regions[i]);
    }
    return solution;
}
