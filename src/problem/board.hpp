#ifndef TILER_PROBLEM_BOARD_HPP_
#define TILER_PROBLEM_BOARD_HPP_

#include <ostream>
#include <string>

#include "problem/region.hpp"

// Represents the board that is expected to be tiled by polyomino tiles.
class Board {
public:
    Board() = delete;
    Board(Region region);
    Board(std::string shape);
    friend std::ostream &operator<<(std::ostream &os, const Board &board);

private:
    Region region_;
};

#endif  // TILER_PROBLEM_BOARD_HPP_
