#include "solvers/ilp_solver.hpp"

#include <memory>
#include <vector>

#include "print.hpp"
#include "problem/problem.hpp"
#include "solution/solution.hpp"
#include "solvers/ilp_utils/coin_cbc_wrapper.hpp"
#include "solvers/ilp_utils/ilp_utils.hpp"

IlpSolver::IlpSolver(Problem problem, std::unique_ptr<CoinCbcWrapper> coin_cbc_wrapper,
                     ilp_utils::ConstraintSense cell_sense, ilp_utils::ObjectiveSense obj_sense)
        : problem_(problem),
          coin_cbc_wrapper_(std::move(coin_cbc_wrapper)),
          cell_sense_(cell_sense),
          obj_sense_(obj_sense) {
    problem_.limit_tile_counts();
}

Solution IlpSolver::solve(bool print_stats) {
    using namespace ilp_utils;

    int w = problem_.board_.get_width();
    int h = problem_.board_.get_height();
    std::vector<std::vector<Vars>> cell_vars(h, std::vector<Vars>(w, Vars{}));
    std::vector<PlacedRegion> placed_regions;
    for (const Tile& tile : problem_.tiles_) {
        Vars tile_vars;
        for (auto [bx, by] : problem_.board_.get_cells()) {
            for (const Region& region : tile) {
                int sx = bx - region.get_top_left_x();
                int sy = by - region.get_top_left_y();
                if (!problem_.board_.has_subregion(sx, sy, region)) continue;

                placed_regions.push_back({sx, sy, region});
                Var var = coin_cbc_wrapper_->new_var(true, 0.0, 1.0, tile.get_size());
                tile_vars.push_back(var.with_coeff(1.0));
                for (auto [rx, ry] : region.get_cells()) {
                    cell_vars[sy + ry][sx + rx].push_back(var.with_coeff(1.0));
                }
            }
        }
        if (tile_vars.size() > 0 && tile.get_count() > 0) {
            coin_cbc_wrapper_->add_constraint(
                    Constraint(tile_vars, ConstraintSense::kLeq, tile.get_count()));
        }
    }

    for (auto [x, y] : problem_.board_.get_cells()) {
        if (cell_vars[y][x].size() == 0) {
            // the cell cannot be covered, the problem is unsolvable
            return {};
        }
        coin_cbc_wrapper_->add_constraint(Constraint(cell_vars[y][x], cell_sense_, 1.0));
    }

    if (print_stats) {
        print::stats() << placed_regions.size() << " placed regions\n"
                       << coin_cbc_wrapper_->get_var_count() << " variables\n"
                       << coin_cbc_wrapper_->get_constraint_count() << " constraints\n";
    } else {
        coin_cbc_wrapper_->disableLogs();
    }

    bool result = coin_cbc_wrapper_->solve(obj_sense_, problem_.board_.get_size());
    if (!result) return {};

    Solution solution;
    std::vector<double> var_values = coin_cbc_wrapper_->get_solution();
    for (int i = 0; i < static_cast<int>(placed_regions.size()); i++) {
        if (approxEq(var_values[i], 1.0)) {
            solution.push_back(placed_regions[i]);
        }
    }
    return solution;
}
