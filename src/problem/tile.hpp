#ifndef TILER_PROBLEM_TILE_HPP_
#define TILER_PROBLEM_TILE_HPP_

#include <ostream>
#include <string>
#include <vector>

#include "problem/region.hpp"

// Represents a polyomino tile with all it's rotations/reflections as well as the available number
// of pieces of this tile.
class Tile {
private:
    std::vector<Region> regions_;
    int count_;  // -1 represents infinity

public:
    Tile() = delete;

    // If `reflection` is set to true, tile will represent not only it's rotations, but also all
    // rotations of the reflected shape.
    Tile(Region region, int count, bool reflection);

    // Parses the tile definitions, consisting of a shape definition optionally prefixed with 'N:'
    // where N is the number of available pieces (otherwise set to infinity).
    static Tile parse(std::string s, bool reflection);

    friend std::ostream &operator<<(std::ostream &os, const Tile &tile);
};

#endif  // TILER_PROBLEM_TILE_HPP_
