#ifndef TILER_SOLUTION_SOLUTION_HPP_
#define TILER_SOLUTION_SOLUTION_HPP_

#include <ostream>
#include <string>
#include <vector>

#include "problem/problem.hpp"
#include "solution/placed_region.hpp"

// Contains the regions used in the solution together with their coordinates. The regions' order
// can be arbitrary.
class Solution : public std::vector<PlacedRegion> {
public:
    void save_image(std::string filepath, Problem problem);
    friend std::ostream &operator<<(std::ostream &os, const Solution &solution);

private:
    // Size of a cell in pixels (does not matter much, because the generated image is SVG).
    static const int kCellSize;

    // Colors of named tiles (in their lexicograpical order).
    static const std::vector<std::string> kColors;
};

#endif  // TILER_SOLUTION_SOLUTION_HPP_
