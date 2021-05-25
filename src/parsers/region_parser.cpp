#include "parsers/region_parser.hpp"

#include <regex>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "parse_error.hpp"
#include "problem/region.hpp"
#include "utils.hpp"

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
    // holes are allowed for now
    // if (utils::has_hole(w, h, matrix)) {
    //     throw ParseError("Not a valid shape definition - shape has a hole:\n" + s);
    // }
    return Region(w, h, matrix);
}
