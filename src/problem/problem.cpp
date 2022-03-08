#include "problem/problem.hpp"

#include <vector>

#include "print.hpp"
#include "problem/board.hpp"
#include "problem/tile.hpp"

Problem::Problem(Board board, std::vector<Tile> tiles) : board_(board), tiles_(tiles) {}

void Problem::limit_tile_counts() {
    int board_size = board_.get_size();
    for (Tile& tile : tiles_) {
        tile.limit_count(board_size / tile.get_size());
    }
}

void Problem::print() const {
    board_.print();
    print::normal_bold() << "\nTILES";
    for (const Tile& tile : tiles_) {
        print::normal() << "\n";
        tile.print();
    }
}
