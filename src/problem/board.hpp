#ifndef TILER_PROBLEM_BOARD_HPP_
#define TILER_PROBLEM_BOARD_HPP_

#include <ostream>
#include <string>

#include "problem/region.hpp"

// Represents the board that is expected to be tiled by polyomino tiles.
class Board : public Region {
public:
    Board() = delete;
    Board(Region region);

    void print() const;
};

#endif  // TILER_PROBLEM_BOARD_HPP_
