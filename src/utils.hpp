#ifndef TILER_UTILS_HPP_
#define TILER_UTILS_HPP_

#include <set>
#include <tuple>
#include <vector>

namespace utils {

using BoolMatrix = std::vector<std::vector<bool>>;

// Represents an edge between two matrix cells. Value of [dx, dy] is always either [0, 1] or [1, 0].
// Edge [x, y, dx, dy] starts at the top-left corner of the cell at [x, y] and it's direction is
// given by dx and dy.
struct Edge {
    int x;
    int y;
    int dx;
    int dy;

    inline bool operator<(const Edge &other) const {
        return std::tie(x, y, dx, dy) < std::tie(other.x, other.y, other.dx, other.dy);
    };
};

// Starts at `start_x`, `start_y` and floods (ie. negates) all accessible cells (of the same value).
// Crossing `edges` is forbidden.
BoolMatrix flood_fill(int start_x, int start_y, int w, int h, BoolMatrix matrix,
                      std::set<Edge> edges = {});

// Checks if `matrix` contains `value`.
bool matrix_contains(BoolMatrix matrix, bool value);

// Removes all leading and trailing rows and columns of `matrix` containing only `false` values,
// returns new width, height and matrix.
std::tuple<int, int, BoolMatrix> remove_margins(int w, int h, BoolMatrix matrix);

// Checks if the region of true cells in `matrix` is continuous (only edge-adjacent neighbors are
// taken into account).
bool is_continuous(int w, int h, BoolMatrix matrix);

// Checks if the region of true cells in `matrix` has a "hole" - at least one false cell inside it.
bool has_hole(int w, int h, BoolMatrix matrix);

}  // namespace utils

#endif  // TILER_UTILS_HPP_
