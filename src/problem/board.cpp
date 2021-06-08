#include "problem/board.hpp"

#include "print.hpp"
#include "problem/region.hpp"

Board::Board(Region region) : Region(region) {}

void Board::print() const {
    print::normal_bold() << "BOARD\n";
    Region::print();
    print::normal() << "(fits inside " << get_width() << "x" << get_height() << " grid)\n";
}
