#include "problem/tile.hpp"

#include <algorithm>
#include <ostream>

#include "parse_error.hpp"
#include "print.hpp"
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

void Tile::print() const {
    switch (count_) {
        case -1:
            print::normal() << "Unlimited number of:";
            break;
        case 1:
            print::normal() << "1 piece of:";
            break;
        default:
            print::normal() << count_ << " pieces of:";
            break;
    }
    regions_[0].print();
    if (regions_.size() > 1) {
        print::normal() << "\n(" << regions_.size() << " possible rotations/reflections)";
    }
}
