#include "problem/board.hpp"

#include <ostream>

#include "print.hpp"
#include "problem/region.hpp"

Board::Board(Region region) : Region(region) {}

void Board::print() const {
    print::normal() << "BOARD\n";
    Region::print();
    print::normal() << "\n(fits inside " << get_width() << "x" << get_height() << " grid)";
}
