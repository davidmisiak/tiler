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
    // rotations of the reflected shape. The value `kCountInfinity` of `count` means infinity.
    Tile(Region region, int count, bool reflection);

    void print() const;

    inline int get_size() const { return regions_[0].get_size(); };
    inline int get_count() const { return count_; };

    // Adds `d` to the cell count (`d` may be negative). If the cell count is infinity, no
    // modification is performed.
    inline void add_count(int d) { count_ == kCountInfinity || (count_ += d); };

    // Set the tile count to `m` if it exceeds `m`.
    inline void limit_count(int m) {
        count_ = (count_ == kCountInfinity) ? m : std::min(count_, m);
    };

    inline TileConstIterator begin() const { return regions_.begin(); };
    inline TileConstIterator end() const { return regions_.end(); };

    static constexpr int kCountInfinity = -1;

private:
    std::vector<Region> regions_;
    int count_;
};

#endif  // TILER_PROBLEM_TILE_HPP_
