#include "utils.hpp"

#include <algorithm>
#include <queue>
#include <tuple>
#include <vector>

namespace {

// Starts at `start_x`, `start_y` and floods (ie. negates) all accessible cells.
// Note that the not flooded value is the original value of `matrix[start_y][start_x]` and the
// flooded value is its negation.
utils::BoolMatrix flood_fill(int start_x, int start_y, int w, int h, utils::BoolMatrix matrix) {
    std::queue<std::pair<int, int>> todo;
    todo.push({start_x, start_y});
    bool value = matrix[start_y][start_x];
    matrix[start_y][start_x] = !value;
    while (!todo.empty()) {
        auto [x, y] = todo.front();
        todo.pop();
        std::vector<std::pair<int, int>> steps{{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
        for (auto [nx, ny] : steps) {
            if (0 <= nx && nx < w && 0 <= ny && ny < h && matrix[ny][nx] == value) {
                todo.push({nx, ny});
                matrix[ny][nx] = !value;
            }
        }
    }
    return matrix;
}

bool matrix_contains(utils::BoolMatrix matrix, bool value) {
    for (auto row : matrix) {
        if (std::find(row.begin(), row.end(), value) != row.end()) {
            return true;
        }
    }
    return false;
}

}  // namespace

std::tuple<int, int, utils::BoolMatrix> utils::remove_margins(int w, int h,
                                                              utils::BoolMatrix matrix) {
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
    utils::BoolMatrix trimmed;
    for (int y = min_y; y <= max_y; y++) {
        std::vector<bool> row(matrix[y].begin() + min_x, matrix[y].begin() + max_x + 1);
        trimmed.push_back(row);
    }
    return {std::max(0, max_x - min_x + 1), std::max(0, max_y - min_y + 1), trimmed};
}

bool utils::is_continuous(int w, int h, utils::BoolMatrix matrix) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (matrix[y][x]) {
                return !matrix_contains(flood_fill(x, y, w, h, matrix), true);
            }
        }
    }
    return true;  // empty matrix
}

bool utils::has_hole(int w, int h, utils::BoolMatrix matrix) {
    for (int x = 0; x < w; x++) {
        if (!matrix[0][x]) matrix = flood_fill(x, 0, w, h, matrix);
        if (!matrix[h - 1][x]) matrix = flood_fill(x, h - 1, w, h, matrix);
    }
    for (int y = 0; y < h; y++) {
        if (!matrix[y][0]) matrix = flood_fill(0, y, w, h, matrix);
        if (!matrix[y][w - 1]) matrix = flood_fill(w - 1, y, w, h, matrix);
    }
    return matrix_contains(matrix, false);
}
