#include "problem/tile.hpp"

#include <algorithm>
#include <ostream>
#include <regex>
#include <string>
#include <vector>

#include "parse_error.hpp"
#include "problem/region.hpp"

Tile::Tile(Region region, int count, bool reflection) : count_(count) {
    Region current = region;
    for (int ref = 0; ref < (reflection ? 2 : 1); ref++) {
        for (int rot = 0; rot < 4; rot++) {
            if (std::find(regions_.begin(), regions_.end(), current) == regions_.end()) {
                regions_.push_back(current);
            }
            current = Region::rotate(current);
        }
        current = Region::reflect(current);
    }
}

Tile Tile::parse(std::string s, bool reflection) {
    if (s.find(":") == std::string::npos) {
        return Tile(Region::parse(s), -1, reflection);
    }
    std::smatch matches;
    if (std::regex_match(s, matches, std::regex("([1-9][0-9]*):\n?([^:]+)"))) {
        int count = std::stoi(matches[1]);
        return Tile(Region::parse(matches[2]), count, reflection);
    }
    throw ParseError("Not a valid tile definition:\n" + s);
}

std::ostream &operator<<(std::ostream &os, const Tile &tile) {
    switch (tile.count_) {
        case -1:
            os << "Unlimited number of:";
            break;
        case 1:
            os << "1 piece of:";
            break;
        default:
            os << tile.count_ << " pieces of:";
            break;
    }
    os << "\n" << tile.regions_[0];
    if (tile.regions_.size() > 1) {
        os << "\n(" << tile.regions_.size() << " possible rotations/reflections)";
    }
    return os;
}
