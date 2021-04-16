#ifndef TILER_SOLUTION_SOLUTION_HPP_
#define TILER_SOLUTION_SOLUTION_HPP_

#include <vector>

#include "solution/placed_region.hpp"

// Contains the regions used in the solution together with their coordinates. The regions' order
// can be arbitrary.
using Solution = std::vector<PlacedRegion>;

#endif  // TILER_SOLUTION_SOLUTION_HPP_
