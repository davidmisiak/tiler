#include "problem/board.hpp"

#include <ostream>

#include "problem/region.hpp"

Board::Board(Region region) : Region(region) {}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    os << "BOARD\n" << static_cast<Region>(board);
    os << "\n(fits inside " << board.get_width() << "x" << board.get_height() << " grid)";
    return os;
}
