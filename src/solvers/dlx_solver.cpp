#include "solvers/dlx_solver.hpp"

#include <vector>

#include "dlx_utils/dlx_wrapper.hpp"
#include "print.hpp"
#include "problem/problem.hpp"
#include "problem/region.hpp"
#include "problem/tile.hpp"
#include "solution/placed_region.hpp"
#include "solution/solution.hpp"

DlxSolver::DlxSolver(Problem problem, DlxWrapper dlx_wrapper)
        : problem_(problem), dlx_wrapper_(dlx_wrapper) {
    problem_.limit_tile_counts();
}

// The principle is similar to SatAmoSolver. However, instead of cell clauses, we have cell items to
// cover; instead of instance clauses, we have instance items to (optionally) cover; and instead of
// tile placement boolean variables, we have covering options (item subsets). Each tile placement
// option contains the corresponding cell items and the tile instance item.
Solution DlxSolver::solve(bool print_stats, [[maybe_unused]] int max_seconds) {
    int w = problem_.board_.get_width();
    int h = problem_.board_.get_height();
    bool exact_tile_set = problem_.extra_tile_square_count() == 0;
    std::vector<std::vector<int>> cell_items(h, std::vector<int>(w, -1));
    std::vector<PlacedRegion> placed_regions;

    auto board_cells = problem_.board_.get_cells();
    for (auto [x, y] : board_cells) {
        cell_items[y][x] = dlx_wrapper_.new_primary_item();
    }

    for (const Tile& tile : problem_.tiles_) {
        int tile_count = tile.get_count();
        int add_instance_item = true;
        if (tile_count * tile.get_size() >= problem_.board_.get_size()) {
            // If there are enough instances of the single tile to cover the board, we can drop the
            // entire notion of tile instances and just encode cell covering.
            tile_count = 1;
            add_instance_item = false;
        }

        for (int i = 0; i < tile_count; i++) {
            int instance_item = -1;  // dummy value
            if (add_instance_item) {
                // If `exact_tile_set` is true, we know that each tile instance has to be used,
                // let's tell this to the DLX algorithm.
                instance_item = exact_tile_set ? dlx_wrapper_.new_primary_item()
                                               : dlx_wrapper_.new_secondary_item();
            }

            int position_number = 0;
            for (auto [bx, by] : board_cells) {
                position_number++;
                if (position_number <= i) {
                    // Minor optimization - `i`-th tile can be placed only on the `i`-th position or
                    // later.
                    continue;
                }

                for (const Region& region : tile) {
                    int sx = bx - region.get_top_left_x();
                    int sy = by - region.get_top_left_y();
                    if (!problem_.board_.has_subregion(sx, sy, region)) continue;

                    std::vector<int> option;
                    if (add_instance_item) {
                        option.push_back(instance_item);
                    }
                    for (auto [rx, ry] : region.get_cells()) {
                        option.push_back(cell_items[sy + ry][sx + rx]);
                    }
                    int tag = static_cast<int>(placed_regions.size());
                    dlx_wrapper_.add_option(option, tag);
                    placed_regions.push_back({sx, sy, region});
                }
            }
        }
    }

    if (print_stats) {
        print::stats() << placed_regions.size() << " placed regions\n";
        dlx_wrapper_.print_stats();
    }

    std::vector<int> solution_tags = dlx_wrapper_.solve();
    if (solution_tags.empty()) return {};

    Solution solution;
    for (int tag : solution_tags) {
        solution.push_back(placed_regions[tag]);
    }
    return solution;
}
