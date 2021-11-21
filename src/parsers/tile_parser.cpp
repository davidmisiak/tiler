#include "parsers/tile_parser.hpp"

#include <regex>
#include <string>

#include "parse_error.hpp"
#include "parsers/region_parser.hpp"
#include "problem/tile.hpp"

Tile tile_parser::parse(std::string s, bool reflection) {
    if (s.find(":") == std::string::npos) {
        return Tile(region_parser::parse(s), Tile::kCountInfinity, reflection);
    }
    std::smatch matches;
    if (std::regex_match(s, matches, std::regex("([1-9][0-9]*):\n?([^:]+)"))) {
        int count = std::stoi(matches[1]);
        return Tile(region_parser::parse(matches[2]), count, reflection);
    }
    throw ParseError("Not a valid tile definition:\n" + s);
}
