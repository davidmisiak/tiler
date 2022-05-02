#include "solvers/simple/simple_solver.hpp"

#include <algorithm>
#include <vector>

#include "print.hpp"
#include "problem/problem.hpp"
#include "problem/tile.hpp"
#include "solution/solution.hpp"
#include "solvers/simple/variant_ordering.hpp"

SimpleSolver::SimpleSolver(Problem problem, variant_ordering::Ordering ordering)
        : problem_(problem), ordering_(ordering) {
    for (int i = 0; i < static_cast<int>(problem_.tiles_.size()); i++) {
        for (const Region& region : problem_.tiles_[i]) {
            variant_refs_.push_back({region, i});
        }
    }
}

// Uses backtracking. In every recursive call, every orientation/reflection of every available tile
// is tried - we try to place the left-most unit square of it's top-most row to the left-most empty
// unit square of board's top-most row with empty squares. If there is available space for the
// entire tile, a recursive call is made. If no fit is found, this branch returns false. The order
// in which tile variants are tried is determined by `ordering_`.
Solution SimpleSolver::solve(bool print_stats, [[maybe_unused]] int max_seconds) {
    step();
    if (print_stats) {
        print::stats() << nodes_ << " nodes\n";
    }
    return solution_;
}

bool SimpleSolver::step() {
    nodes_++;
    if (problem_.board_.get_size() == 0) {
        return true;
    }
    int top_left_x = problem_.board_.get_top_left_x();
    int top_left_y = problem_.board_.get_top_left_y();

    std::vector<variant_ordering::VariantRef> variant_refs_backup;
    const auto [should_sort, comparator] = ordering_(problem_.tiles_);
    if (should_sort) {
        variant_refs_backup = variant_refs_;
        std::stable_sort(variant_refs_.begin(), variant_refs_.end(), comparator);
    }
    for (const auto& [region, tile_index] : variant_refs_) {
        Tile& tile = problem_.tiles_[tile_index];
        if (tile.get_count() == 0) continue;

        int x = top_left_x - region.get_top_left_x();
        int y = top_left_y - region.get_top_left_y();
        if (!problem_.board_.has_subregion(x, y, region)) continue;
        problem_.board_.remove_subregion(x, y, region);
        tile.add_count(-1);
        if (step()) {
            solution_.push_back({x, y, region});
            if (should_sort) variant_refs_ = variant_refs_backup;
            return true;
        }
        problem_.board_.add_subregion(x, y, region);
        tile.add_count(1);
    }
    if (should_sort) variant_refs_ = variant_refs_backup;
    return false;
}
