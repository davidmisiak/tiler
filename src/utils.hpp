#ifndef TILER_UTILS_HPP_
#define TILER_UTILS_HPP_

#include <tuple>
#include <vector>

namespace utils {

// Removes all leading and trailing rows and columns of `matrix` containing only `false` values,
// returns new width, height and matrix.
std::tuple<int, int, std::vector<std::vector<bool>>> remove_margins(
        int w, int h, std::vector<std::vector<bool>> matrix);

}  // namespace utils

#endif  // TILER_UTILS_HPP_
