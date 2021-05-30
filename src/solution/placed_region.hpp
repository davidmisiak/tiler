#ifndef TILER_SOLUTION_PLACED_REGION_HPP_
#define TILER_SOLUTION_PLACED_REGION_HPP_

#include "problem/region.hpp"

// Represents a region placed on a board. `x` and `y` are coordinates of the top-left corner of
// the `region`'s surrounding rectangle.
struct PlacedRegion {
    const int x;
    const int y;
    const Region region;

    inline bool operator==(const PlacedRegion &other) const {
        return x == other.x && y == other.y && region == other.region;
    };
};

#endif  // TILER_SOLUTION_PLACED_REGION_HPP_
