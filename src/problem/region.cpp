#include "problem/region.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "help_strings.hpp"
#include "print.hpp"
#include "utils.hpp"

Region::Region(int w, int h, utils::BoolMatrix matrix)
        : w_(w), h_(h), matrix_(matrix), size_(0), top_left_x_(-1), top_left_y_(-1) {
    for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
            if (!matrix_[y][x]) continue;
            size_++;
            if (top_left_x_ == -1) {
                top_left_x_ = x;
                top_left_y_ = y;
            }
        }
    }
}

Region Region::rotate() const {
    utils::BoolMatrix rotated(w_, std::vector<bool>(h_, false));
    for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
            rotated[w_ - 1 - x][y] = matrix_[y][x];
        }
    }
    return Region(h_, w_, rotated);
}

Region Region::reflect() const {
    utils::BoolMatrix reflected(matrix_);
    for (int y = 0; y < h_; y++) {
        std::reverse(reflected[y].begin(), reflected[y].end());
    }
    return Region(w_, h_, reflected);
}

void Region::print() const {
    if (w_ > help_strings::kMaxWidth || h_ > help_strings::kMaxWidth) {
        print::normal() << "(too big to show)\n";
        return;
    }
    for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
            print::shape() << (matrix_[y][x] ? 'x' : ' ');
        }
        print::normal() << "\n";
    }
}

bool Region::has_subregion(int origin_x, int origin_y, const Region &region) const {
    if (origin_x < 0 || origin_y < 0 || origin_x + region.w_ > w_ || origin_y + region.h_ > h_) {
        return false;
    }
    for (int y = 0; y < region.h_; y++) {
        for (int x = 0; x < region.w_; x++) {
            if (region.matrix_[y][x] && !matrix_[origin_y + y][origin_x + x]) {
                return false;
            }
        }
    }
    return true;
}

void Region::remove_subregion(int origin_x, int origin_y, const Region &region) {
    for (int y = 0; y < region.h_; y++) {
        for (int x = 0; x < region.w_; x++) {
            if (region.matrix_[y][x]) {
                matrix_[origin_y + y][origin_x + x] = false;
            }
        }
    }
    update_top_left(origin_x, origin_y);
    size_ -= region.size_;
}

void Region::add_subregion(int origin_x, int origin_y, const Region &region) {
    for (int y = 0; y < region.h_; y++) {
        for (int x = 0; x < region.w_; x++) {
            if (region.matrix_[y][x]) {
                matrix_[origin_y + y][origin_x + x] = true;
            }
        }
    }
    update_top_left(origin_x, origin_y);
    size_ += region.size_;
}

std::vector<std::pair<int, int>> Region::get_cells() const {
    std::vector<std::pair<int, int>> cells;
    for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
            if (matrix_[y][x]) {
                cells.push_back({x, y});
            }
        }
    }
    return cells;
}

std::vector<std::pair<int, int>> Region::get_missing_cells() const {
    std::vector<std::pair<int, int>> cells;
    for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
            if (!matrix_[y][x]) {
                cells.push_back({x, y});
            }
        }
    }
    return cells;
}

std::vector<utils::Edge> Region::get_edges() const {
    std::vector<utils::Edge> edges;
    for (int y = 0; y <= h_; y++) {
        for (int x = 0; x <= w_; x++) {
            bool curr = (y < h_ && x < w_) ? matrix_[y][x] : false;
            bool left = (y < h_ && x > 0) ? matrix_[y][x - 1] : false;
            bool above = (y > 0 && x < w_) ? matrix_[y - 1][x] : false;
            if (curr ^ left) edges.push_back({x, y, 0, 1});
            if (curr ^ above) edges.push_back({x, y, 1, 0});
        }
    }
    return edges;
}

void Region::update_top_left(int from_x, int from_y) {
    if (top_left_x_ != -1) {
        if (top_left_y_ < from_y || (top_left_y_ == from_y && top_left_x_ < from_x)) {
            return;
        }
    }
    for (int y = from_y; y < h_; y++) {
        for (int x = (y == from_y) ? from_x : 0; x < w_; x++) {
            if (matrix_[y][x]) {
                top_left_x_ = x;
                top_left_y_ = y;
                return;
            }
        }
    }
    top_left_x_ = -1;
    top_left_y_ = -1;
}

// clang-format off
const std::map<std::string, std::string> Region::kNamedShapes{
    {"1", "x"},
    {"2", "xx"},
    {"3I", "xxx"},
    {"3L",
        "x \n"
        "xx"},
    {"4I", "xxxx"},
    {"4J",
        "xxx\n"
        "  x"},
    {"4L",
        "xxx\n"
        "x  "},
    {"4O",
        "xx\n"
        "xx"},
    {"4S",
        " xx\n"
        "xx "},
    {"4T",
        "xxx\n"
        " x "},
    {"4Z",
        "xx \n"
        " xx"},
    {"5A",
        "xxxx\n"
        "  x "},
    {"5F",
        " xx\n"
        "xx \n"
        " x "},
    {"5G",
        " xxx\n"
        "xx  "},
    {"5I", "xxxxx"},
    {"5J",
        "xxxx\n"
        "   x"},
    {"5L",
        "xxxx\n"
        "x   "},
    {"5N",
        "xxx \n"
        "  xx"},
    {"5P",
        "xxx\n"
        " xx"},
    {"5Q",
        "xxx\n"
        "xx "},
    {"5R",
        "xx \n"
        " xx\n"
        " x "},
    {"5S",
        " xx\n"
        " x \n"
        "xx "},
    {"5T",
        "xxx\n"
        " x \n"
        " x "},
    {"5U",
        "x x\n"
        "xxx"},
    {"5V",
        "x  \n"
        "x  \n"
        "xxx"},
    {"5W",
        "x  \n"
        "xx \n"
        " xx"},
    {"5X",
        " x \n"
        "xxx\n"
        " x "},
    {"5Y",
        "xxxx\n"
        " x  "},
    {"5Z",
        "xx \n"
        " x \n"
        " xx"},
};
// clang-format on
