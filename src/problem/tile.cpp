#include "problem/tile.hpp"

#include <algorithm>
#include <ostream>

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
