#ifndef TILER_UTILS_HPP_
#define TILER_UTILS_HPP_

#include <tuple>
#include <vector>

namespace utils {

// Removes all leading and trailing rows and columns of `matrix` containing only `false` values,
// returns new width, height and matrix.
std::tuple<int, int, std::vector<std::vector<bool>>> remove_margins(
        int w, int h, std::vector<std::vector<bool>> matrix);

// Checks if the region of true cells in `matrix` is continuous (only edge-adjacent neighbors are
// taken into account).
bool is_continuous(int w, int h, std::vector<std::vector<bool>> matrix);

// Checks if the region of true cells in `matrix` has a "hole" - at least one false cell inside it.
bool has_hole(int w, int h, std::vector<std::vector<bool>> matrix);

}  // namespace utils

#endif  // TILER_UTILS_HPP_
