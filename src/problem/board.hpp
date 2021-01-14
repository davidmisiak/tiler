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
    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};

#endif  // TILER_PROBLEM_BOARD_HPP_
