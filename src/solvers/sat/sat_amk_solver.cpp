#include "solvers/sat/sat_amk_solver.hpp"

#include <memory>
#include <vector>

#include "print.hpp"
#include "problem/problem.hpp"
#include "problem/region.hpp"
#include "problem/tile.hpp"
#include "solution/placed_region.hpp"
#include "solution/solution.hpp"
#include "solvers/sat/sat_preprocessor.hpp"
#include "solvers/sat/sat_utils.hpp"
#include "solvers/sat/sat_wrapper.hpp"
#include "utils.hpp"

SatAmkSolver::SatAmkSolver(Problem problem, std::unique_ptr<SatWrapper> sat_wrapper,
                           std::unique_ptr<SatPreprocessor> preprocessor,
                           PBLibWrapper pblib_wrapper)
        : problem_(problem),
          sat_wrapper_(std::move(sat_wrapper)),
          preprocessor_(std::move(preprocessor)),
          pblib_wrapper_(std::move(pblib_wrapper)) {
    problem_.limit_tile_counts();
}

// For every possible position and orientation of each tile, there is one logical variable
// representing "this tile is placed on the board on this position with this orietation". Instead of
// having such variables for each tile instance and encoding that each tile instance has at most one
// position (like SatAmoSolver does), this solver encodes that the tile is present on at most k
// positions, where k is the tile instance count.
Solution SatAmkSolver::solve(bool print_stats, int max_seconds) {
    using sat_utils::Lit, sat_utils::Clause;

    int w = problem_.board_.get_width();
    int h = problem_.board_.get_height();
    bool exact_tile_set = problem_.extra_tile_square_count() == 0;
    std::vector<std::pair<Clause, int>> tile_clauses;
    std::vector<std::vector<Clause>> cell_clauses(h, std::vector<Clause>(w, Clause{}));
    std::vector<PlacedRegion> placed_regions;

    auto board_cells = problem_.board_.get_cells();
    for (const Tile& tile : problem_.tiles_) {
        Clause tile_clause;
        for (auto [bx, by] : board_cells) {
            for (const Region& region : tile) {
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
        int tile_count = tile.get_count();
        if (tile_count * tile.get_size() < problem_.board_.get_size()) {
            // We need to enforce the maximum tile instance count only if there is not enough tile
            // instances to cover the entire board.
            tile_clauses.push_back({tile_clause, tile_count});
        }
    }

    for (const auto& [tile_clause, tile_count] : tile_clauses) {
        if (tile_clause.size() == 0 || tile_count == 0) continue;
        if (exact_tile_set) {
            // We know that each tile instance has to be used, let's tell this to the SAT solver.
            pblib_wrapper_.exactly_k(tile_clause, sat_wrapper_, tile_count);
        } else {
            pblib_wrapper_.at_most_k(tile_clause, sat_wrapper_, tile_count);
        }
    }

    for (auto [x, y] : board_cells) {
        if (cell_clauses[y][x].size() == 0) {
            // the cell cannot be covered, the problem is unsolvable
            return {};
        }
        pblib_wrapper_.exactly_k(cell_clauses[y][x], sat_wrapper_, 1);
    }

    preprocessor_->run(sat_wrapper_, max_seconds);

    if (print_stats) {
        print::stats() << placed_regions.size() << " placed regions\n";
        sat_wrapper_->print_stats();
        preprocessor_->print_stats();
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
