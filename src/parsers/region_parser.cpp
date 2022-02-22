#include "parsers/region_parser.hpp"

#include <algorithm>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "parse_error.hpp"
#include "problem/region.hpp"
#include "utils.hpp"

namespace {

Region border_to_region(std::vector<std::pair<int, int>> steps) {
    int max_u = 0, max_d = 0, max_l = 0, max_r = 0;
    for (auto [sx, sy] : steps) {
        if (sx < 0) max_l++;
        if (sx > 0) max_r++;
        if (sy < 0) max_u++;
        if (sy > 0) max_d++;
    }
    int w = std::max(max_l, max_r) + 1;
    int h = std::max(max_u, max_d) + 1;
    int x = w;
    int y = h;
    std::set<std::pair<int, int>> visited;
    std::set<utils::Edge> edges;
    for (auto [sx, sy] : steps) {
        if (visited.count({x, y})) {
            throw ParseError("Not a valid shape definition - border intersection detected:\n");
        }
        visited.insert({x, y});
        int new_x = x + sx;
        int new_y = y + sy;
        utils::Edge e = (sx < 0 || sy < 0) ? utils::Edge{new_x, new_y, -sx, -sy}
                                           : utils::Edge{x, y, sx, sy};
        edges.insert(e);
        x = new_x;
        y = new_y;
    }
    if (x != w || y != h) {
        throw ParseError("Not a valid shape definition - path is not correcty closed:\n");
    }

    utils::BoolMatrix matrix(2 * h, std::vector<bool>(2 * w, false));
    auto [cx, cy] = *std::min_element(visited.begin(), visited.end());
    matrix = utils::flood_fill(cx, cy, 2 * w, 2 * h, matrix, edges);
    auto [rw, rh, rmatrix] = utils::remove_margins(2 * w, 2 * h, matrix);
    return Region(rw, rh, rmatrix);
}

}  // namespace

Region region_parser::parse_raw(const std::string s) {
    // named region, eg. "4O"
    if (Region::kNamedShapes.count(s)) {
        return region_parser::parse(Region::kNamedShapes.at(s));
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
        for (const std::string& line : lines) {
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
    // border path, eg. "DDRRULUL"
    if (std::regex_match(s, std::regex("[UDLR]+"))) {
        std::map<char, std::pair<int, int>> directions = {
                {'U', {0, -1}}, {'D', {0, 1}}, {'L', {-1, 0}}, {'R', {1, 0}}};
        std::vector<std::pair<int, int>> steps;
        std::transform(s.begin(), s.end(), std::back_inserter(steps),
                       [&](const char& c) { return directions[c]; });
        try {
            return border_to_region(steps);
        } catch (const ParseError& e) {
            throw ParseError(e.what() + s);
        }
    }
    throw ParseError("Not a valid shape definition:\n" + s);
}

Region region_parser::parse(const std::string s) {
    Region raw = parse_raw(s);
    auto [w, h, matrix] =
            utils::remove_margins(raw.get_width(), raw.get_height(), raw.get_matrix());
    if (w == 0 || h == 0) {
        throw ParseError(
                "Not a valid shape definition - a shape appears to be empty.\n"
                "Check your file with problem assignment for extra spaces.");
    }
    if (!utils::is_continuous(w, h, matrix)) {
        throw ParseError("Not a valid shape definition - shape is not continuous:\n" + s);
    }
    // holes in shape definitions are allowed for now
    // if (utils::has_hole(w, h, matrix)) {
    //     throw ParseError("Not a valid shape definition - shape has a hole:\n" + s);
    // }
    return Region(w, h, matrix);
}
