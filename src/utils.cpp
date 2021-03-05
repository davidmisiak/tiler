#include "utils.hpp"

#include <tuple>
#include <vector>

std::tuple<int, int, std::vector<std::vector<bool>>> utils::remove_margins(
        int w, int h, std::vector<std::vector<bool>> matrix) {
    int max_x = -1;
    int min_x = w;
    int max_y = -1;
    int min_y = h;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (matrix[y][x]) {
                max_x = std::max(max_x, x);
                min_x = std::min(min_x, x);
                max_y = std::max(max_y, y);
                min_y = std::min(min_y, y);
            }
        }
    }
    std::vector<std::vector<bool>> trimmed;
    for (int y = min_y; y <= max_y; y++) {
        std::vector<bool> row(matrix[y].begin() + min_x, matrix[y].begin() + max_x + 1);
        trimmed.push_back(row);
    }
    return {std::max(0, max_x - min_x + 1), std::max(0, max_y - min_y + 1), trimmed};
}
