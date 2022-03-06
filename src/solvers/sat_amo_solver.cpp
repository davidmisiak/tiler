#include "solvers/sat_amo_solver.hpp"

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

SatAmoSolver::SatAmoSolver(Problem problem, std::unique_ptr<SatWrapper> sat_wrapper,
                           std::unique_ptr<SymmetryBreaker> symmetry_breaker,
                           PBLibWrapper pblib_wrapper)
        : problem_(problem),
          sat_wrapper_(std::move(sat_wrapper)),
          symmetry_breaker_(std::move(symmetry_breaker)),
          pblib_wrapper_(std::move(pblib_wrapper)) {
    int board_size = problem_.board_.get_size();
    for (Tile& tile : problem_.tiles_) {
        tile.limit_count(board_size / tile.get_size());
    }
}

// For every possible position and orientation of every available piece of every tile, there is
// one logical variable representing "this piece is placed on the board on this position with this
// orietation".
// For each piece of each tile, there is a set of clauses that guarantee that at most one of the
// piece's variables is true (ie. each piece is placed on the board at most once).
// For each cell of the board, there is a set of clauses that guarantee that exactly one of the tile
// pieces covers this cell.
// Put together, the CNF formula is satisfiable if and only if the board can be tiled.
Solution SatAmoSolver::solve(bool print_stats) {
    using sat_utils::Lit, sat_utils::Clause;

    int w = problem_.board_.get_width();
    int h = problem_.board_.get_height();
    std::vector<Clause> instance_clauses;
    std::vector<std::vector<Clause>> cell_clauses(h, std::vector<Clause>(w, Clause{}));
    std::vector<PlacedRegion> placed_regions;
    for (const Tile& tile : problem_.tiles_) {
        for (int i = 0; i < tile.get_count(); i++) {
            Clause instance_clause;
            for (auto [bx, by] : problem_.board_.get_cells()) {
                for (const Region& region : tile) {
                    int sx = bx - region.get_top_left_x();
                    int sy = by - region.get_top_left_y();
                    if (!problem_.board_.has_subregion(sx, sy, region)) continue;

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
    }

    for (const Clause& instance_clause : instance_clauses) {
        if (instance_clauses.size() > 0) {
            pblib_wrapper_.at_most_k(instance_clause, sat_wrapper_, 1);
        }
    }

    for (auto [x, y] : problem_.board_.get_cells()) {
        if (cell_clauses[y][x].size() == 0) {
            // the cell cannot be covered, the problem is unsolvable
            return {};
        }
        pblib_wrapper_.exactly_k(cell_clauses[y][x], sat_wrapper_, 1);
    }

    symmetry_breaker_->break_symmetries(sat_wrapper_);

    if (print_stats) {
        print::stats() << placed_regions.size() << " placed regions\n"
                       << sat_wrapper_->get_var_count() << " variables\n"
                       << sat_wrapper_->get_clause_count() << " clauses\n"
                       << sat_wrapper_->get_lit_count() << " literals\n";
        symmetry_breaker_->print_stats();
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
