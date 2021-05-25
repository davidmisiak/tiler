#include "problem/problem.hpp"

#include <iostream>
#include <vector>

#include "problem/board.hpp"
#include "problem/tile.hpp"

Problem::Problem(Board board, std::vector<Tile> tiles) : board_(board), tiles_(tiles) {}

std::ostream &operator<<(std::ostream &os, const Problem &problem) {
    os << problem.board_ << "\n\nTILES";
    for (Tile tile : problem.tiles_) {
        os << "\n\n" << tile;
    }
    os << std::endl;
    return os;
}
