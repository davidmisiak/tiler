#include "solvers/simple_solver.hpp"

#include "print.hpp"
#include "problem/problem.hpp"
#include "problem/tile.hpp"
#include "solution/solution.hpp"

SimpleSolver::SimpleSolver(Problem problem) : problem_(problem) {}

// Uses naive backtracking. In every recursive call, every orientation/reflection of every available
// tile is tried - we try to place the left-most unit square of it's top-most row to the left-most
// empty unit square of board's top-most row with empty squares. If there is available space for the
// entire tile, a recursive call is made. If no fit is found, this branch returns false. Note that
// this algorithm finds a solution if and only if a solution exists.
Solution SimpleSolver::solve(bool print_stats) {
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
    for (Tile& tile : problem_.tiles_) {
        if (tile.get_count() == 0) continue;
        for (const Region& region : tile) {
            int x = top_left_x - region.get_top_left_x();
            int y = top_left_y - region.get_top_left_y();
            if (!problem_.board_.has_subregion(x, y, region)) continue;
            problem_.board_.remove_subregion(x, y, region);
            tile.add_count(-1);
            if (step()) {
                solution_.push_back({x, y, region});
                return true;
            }
            problem_.board_.add_subregion(x, y, region);
            tile.add_count(1);
        }
    }
    return false;
}
