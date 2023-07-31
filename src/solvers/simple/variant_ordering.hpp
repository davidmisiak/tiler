#ifndef TILER_SOLVERS_SIMPLE_VARIANT_ORDERING_HPP_
#define TILER_SOLVERS_SIMPLE_VARIANT_ORDERING_HPP_

#include <functional>
#include <vector>

#include "errors/solve_error.hpp"
#include "problem/region.hpp"
#include "problem/tile.hpp"

namespace variant_ordering {

using VariantRef = std::pair<Region, int>;
using OrderingComparator = std::function<bool(const VariantRef&, const VariantRef&)>;
using Ordering = std::function<std::pair<bool, OrderingComparator>(const std::vector<Tile>&)>;

// Preserves the order of tile variants.
static const Ordering kDefault = [](const std::vector<Tile>& tiles [[maybe_unused]]) {
    return std::pair{
            false,
            [](const VariantRef& a [[maybe_unused]], const VariantRef& b [[maybe_unused]]) {
                throw SolveError("The default variant ordering comparator should not be used.");
                return false;
            },
    };
};

// Prefers variants of tiles with higher instance counts.
static const Ordering kFrequentFirst = [](const std::vector<Tile>& tiles) {
    return std::pair{
            true,
            [&tiles](const VariantRef& a, const VariantRef& b) {
                int a_count = tiles[a.second].get_count();
                int b_count = tiles[b.second].get_count();

                if (b_count == Tile::kCountInfinity) return false;
                if (a_count == Tile::kCountInfinity) return true;
                return a_count > b_count;
            },
    };
};

// Prefers variants of tiles with lower instance counts.
static const Ordering kRareFirst = [](const std::vector<Tile>& tiles) {
    return std::pair{
            true,
            [&tiles](const VariantRef& a, const VariantRef& b) {
                int a_count = tiles[a.second].get_count();
                int b_count = tiles[b.second].get_count();

                if (a_count == Tile::kCountInfinity) return false;
                if (b_count == Tile::kCountInfinity) return true;
                return a_count < b_count;
            },
    };
};

// Prefers tile variants that are able to cover more top-row cells.
static const Ordering kFillTopRow = [](const std::vector<Tile>& tiles) {
    return std::pair{
            true,
            [&tiles](const VariantRef& a, const VariantRef& b) {
                int a_count = a.first.get_row_cell_count(0);
                int b_count = b.first.get_row_cell_count(0);
                return a_count > b_count;
            },
    };
};

}  // namespace variant_ordering

#endif  // TILER_SOLVERS_SIMPLE_VARIANT_ORDERING_HPP_
