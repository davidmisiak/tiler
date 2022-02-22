#include "utils.hpp"

#include <algorithm>
#include <filesystem>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

utils::BoolMatrix utils::flood_fill(int start_x, int start_y, int w, int h,
                                    utils::BoolMatrix matrix, std::set<utils::Edge> edges) {
    std::queue<std::pair<int, int>> todo;
    todo.push({start_x, start_y});
    bool value = matrix[start_y][start_x];
    matrix[start_y][start_x] = !value;
    while (!todo.empty()) {
        auto [x, y] = todo.front();
        todo.pop();
        std::vector<std::pair<int, int>> steps{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (auto [sx, sy] : steps) {
            int nx = x + sx, ny = y + sy;
            if (nx < 0 || nx >= w || ny < 0 || ny >= h || matrix[ny][nx] != value) continue;
            if ((sx == -1 || sy == -1) && edges.count({x, y, -sy, -sx})) continue;
            if ((sx == 1 || sy == 1) && edges.count({nx, ny, sy, sx})) continue;
            todo.push({nx, ny});
            matrix[ny][nx] = !value;
        }
    }
    return matrix;
}

bool utils::matrix_contains(utils::BoolMatrix matrix, bool value) {
    for (auto row : matrix) {
        if (std::find(row.begin(), row.end(), value) != row.end()) {
            return true;
        }
    }
    return false;
}

std::tuple<int, int, utils::BoolMatrix> utils::remove_margins(int w, int h,
                                                              utils::BoolMatrix matrix) {
    int max_x = -1;
    int min_x = w;
    int max_y = -1;
    int min_y = h;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (!matrix[y][x]) continue;
            max_x = std::max(max_x, x);
            min_x = std::min(min_x, x);
            max_y = std::max(max_y, y);
            min_y = std::min(min_y, y);
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
    if (w == 0 || h == 0) {
        return false;
    }
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

bool utils::ends_with(std::string str, std::string suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::vector<std::string> utils::get_file_paths(std::string directory_path) {
    std::vector<std::string> filepaths;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory_path)) {
        if (std::filesystem::is_regular_file(entry)) {
            filepaths.push_back(entry.path().string());
        }
    }
    std::sort(filepaths.begin(), filepaths.end());
    return filepaths;
}
