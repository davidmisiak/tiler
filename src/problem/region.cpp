#include "problem/region.hpp"

#include <algorithm>
#include <ostream>
#include <regex>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "parse_error.hpp"
#include "print.hpp"
#include "utils.hpp"

Region::Region(int w, int h, utils::BoolMatrix matrix)
        : w_(w), h_(h), matrix_(matrix), size_(0), top_left_x_(-1), top_left_y_(-1) {
    for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
            if (matrix_[y][x]) {
                size_++;
                if (top_left_x_ == -1) {
                    top_left_x_ = x;
                    top_left_y_ = y;
                }
            }
        }
    }
}

Region Region::parse_raw(const std::string s) {
    // named region, eg. "4O"
    if (kNamedShapes.count(s)) {
        return Region::parse(kNamedShapes.at(s));
    }
    // dimensions, eg. "2x2"
    std::smatch dimensions_matches;
    if (std::regex_match(s, dimensions_matches, std::regex("([1-9][0-9]*)x([1-9][0-9]*)"))) {
        int w = std::stoi(dimensions_matches[1]);
        int h = std::stoi(dimensions_matches[2]);
        utils::BoolMatrix matrix(h, std::vector<bool>(w, true));
        return Region(w, h, matrix);
    }
    // region map, eg."xx\nxx"
    if (std::regex_match(s, std::regex("( |x|\n)+"))) {
        std::vector<std::string> lines;
        boost::split(lines, s, boost::is_any_of("\n"));
        int w = 0;
        for (std::string line : lines) {
            w = std::max(w, static_cast<int>(line.size()));
        }
        int h = static_cast<int>(lines.size());
        utils::BoolMatrix matrix(h, std::vector<bool>(w, false));
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < std::min(w, static_cast<int>(lines[y].size())); x++) {
                matrix[y][x] = (lines[y][x] == 'x');
            }
        }
        return Region(w, h, matrix);
    }
    throw ParseError("Not a valid shape definition:\n" + s);
}

Region Region::parse(const std::string s) {
    Region raw = parse_raw(s);
    auto [w, h, matrix] = utils::remove_margins(raw.w_, raw.h_, raw.matrix_);
    if (w == 0 || h == 0) {
        throw ParseError(
                "Not a valid shape definition - a shape appears to be empty.\n"
                "Check your file with problem assignment for extra spaces.");
    }
    if (!utils::is_continuous(w, h, matrix)) {
        throw ParseError("Not a valid shape definition - shape is not continuous:\n" + s);
    }
    if (utils::has_hole(w, h, matrix)) {
        throw ParseError("Not a valid shape definition - shape has a hole:\n" + s);
    }
    return Region(w, h, matrix);
}

Region Region::rotate(const Region &region) {
    utils::BoolMatrix rotated(region.w_, std::vector<bool>(region.h_, false));
    for (int y = 0; y < region.h_; y++) {
        for (int x = 0; x < region.w_; x++) {
            rotated[region.w_ - 1 - x][y] = region.matrix_[y][x];
        }
    }
    return Region(region.h_, region.w_, rotated);
}

Region Region::reflect(const Region &region) {
    utils::BoolMatrix reflected(region.matrix_);
    for (int y = 0; y < region.h_; y++) {
        std::reverse(reflected[y].begin(), reflected[y].end());
    }
    return Region(region.w_, region.h_, reflected);
}

bool Region::operator==(const Region &other) const {
    return w_ == other.w_ && h_ == other.h_ && matrix_ == other.matrix_;
}

std::ostream &operator<<(std::ostream &os, const Region &region) {
    if (region.w_ > 70 || region.h_ > 70) {
        os << "(too big to show)";
        return os;
    }
    for (int y = 0; y < region.h_; y++) {
        if (y > 0) os << '\n';
        for (int x = 0; x < region.w_; x++) {
            os << (region.matrix_[y][x] ? 'x' : ' ');
        }
    }
    return os;
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

void Region::update_top_left(int from_x, int from_y) {
    if (top_left_y_ < from_y || (top_left_y_ == from_y && top_left_x_ < from_x)) {
        return;
    }
    for (int y = from_y; y < h_; y++) {
        for (int x = 0 /* sic */; x < w_; x++) {
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
const std::unordered_map<std::string, std::string> Region::kNamedShapes{
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
        " xx\n"
        "xx "},
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
