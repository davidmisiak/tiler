#include "problem/problem.hpp"

#include <iostream>
#include <vector>

#include "print.hpp"
#include "problem/board.hpp"
#include "problem/tile.hpp"

Problem::Problem(Board board, std::vector<Tile> tiles) : board_(board), tiles_(tiles) {}

void Problem::print() const {
    board_.print();
    print::normal() << "\n\nTILES";
    for (Tile tile : tiles_) {
        print::normal() << "\n\n";
        tile.print();
    }
    print::normal() << std::endl;
}
