#ifndef TILER_PROBLEM_TILE_HPP_
#define TILER_PROBLEM_TILE_HPP_

#include <ostream>
#include <vector>

#include "problem/region.hpp"

// Represents a polyomino tile with all it's rotations/reflections as well as the available number
// of pieces of this tile.
class Tile {
public:
    using TileConstIterator = std::vector<Region>::const_iterator;

    Tile() = delete;

    // If `reflection` is set to true, tile will represent not only it's rotations, but also all
    // rotations of the reflected shape.
    Tile(Region region, int count, bool reflection);

    friend std::ostream &operator<<(std::ostream &os, const Tile &tile);

    // -1 represents infinity
    inline int get_count() { return count_; };

    inline TileConstIterator begin() { return regions_.begin(); };
    inline TileConstIterator end() { return regions_.end(); };
    inline void add_count(int d) { count_ == -1 || (count_ += d); };

private:
    std::vector<Region> regions_;
    int count_;
};

#endif  // TILER_PROBLEM_TILE_HPP_
