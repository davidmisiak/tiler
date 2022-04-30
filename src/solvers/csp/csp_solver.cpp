#include "solvers/csp/csp_solver.hpp"

#include <set>
#include <vector>

#include "errors/not_implemented_error.hpp"
#include "print.hpp"
#include "problem/problem.hpp"
#include "problem/region.hpp"
#include "problem/tile.hpp"
#include "solution/placed_region.hpp"
#include "solution/solution.hpp"
#include "solvers/csp/minizinc_wrapper.hpp"

CspSolver::CspSolver(Problem problem, MinizincWrapper minizinc_wrapper)
        : problem_(problem), minizinc_wrapper_(minizinc_wrapper) {
    problem_.limit_tile_counts();
}

Solution CspSolver::solve(bool print_stats, int max_seconds) {
    bool exact_tile_set = problem_.extra_tile_square_count() == 0;
    if (!exact_tile_set) {
        throw NotImplementedError("CSP solver can not solve problems with redundant tiles.");
        // We require no extra tile instances because the model assumes that all tile instances
        // fit into the board.
    }

    int w = problem_.board_.get_width();
    int h = problem_.board_.get_height();

    std::vector<std::pair<int, int>> missingCells = problem_.board_.get_missing_cells();

    std::vector<std::set<int>> variants;
    std::vector<std::set<int>> tiles;
    std::vector<Region> regions;
    int variant_number = 0;
    for (const Tile& tile : problem_.tiles_) {
        if (tile.get_count() == 0) continue;

        std::set<int> tile_variants;
        for (const Region& region : tile) {
            if (region.get_width() > w || region.get_height() > h) continue;

            variant_number++;
            std::set<int> variant;
            for (auto [rx, ry] : region.get_cells()) {
                variant.insert(ry * w + rx + 1);
            }
            variants.push_back(variant);
            tile_variants.insert(variant_number);
            regions.push_back(region);
        }
        for (int i = 0; i < tile.get_count(); i++) {
            tiles.push_back(tile_variants);
        }
    }

    if (print_stats) {
        print::stats() << variants.size() << " variants\n" << tiles.size() << " tiles\n";
    }

    auto [positions, kinds] =
            minizinc_wrapper_.solve(w, h, missingCells, variants, tiles, print_stats, max_seconds);

    if (positions.empty()) {
        return {};
    }

    Solution solution;
    for (int i = 0; i < static_cast<int>(kinds.size()); i++) {
        auto [x, y] = positions[i];
        solution.push_back({x, y, regions[kinds[i] - 1]});
    }
    return solution;
}
