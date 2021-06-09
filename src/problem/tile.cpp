#include "problem/tile.hpp"

#include <algorithm>

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
    regions_[0].print();
    switch (count_) {
        case -1:
            print::normal() << "unlimited pieces";
            break;
        case 1:
            print::normal() << "1 piece";
            break;
        default:
            print::normal() << count_ << " pieces";
            break;
    }
    print::normal() << ", " << regions_.size();
    if (regions_.size() == 1) {
        print::normal() << " rotation/reflection\n";
    } else {
        print::normal() << " rotations/reflections\n";
    }
}
