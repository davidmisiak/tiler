#include "problem/board.hpp"

#include <ostream>
#include <string>

#include "problem/region.hpp"

Board::Board(Region region) : Region(region) {}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    os << "Board:\n" << static_cast<Region>(board);
    os << "\n(fits in " << board.get_width() << "x" << board.get_height() << ")";
    return os;
}
