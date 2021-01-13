#include "problem/board.hpp"

#include <ostream>
#include <string>

#include "problem/region.hpp"

Board::Board(Region region) : region_(region) {}

Board::Board(std::string shape) : region_(Region::parse(shape)) {}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    os << "Board:\n" << board.region_;
    os << "\n(fits in " << board.region_.get_width() << "x" << board.region_.get_height() << ")";
    return os;
}
