#ifndef TILER_PROBLEM_PROBLEM_HPP_
#define TILER_PROBLEM_PROBLEM_HPP_

#include <ostream>
#include <vector>

#include "problem/board.hpp"
#include "problem/tile.hpp"

// Represents the problem assignment - board and list of tiles (with their counts).
class Problem {
public:
    Problem() = delete;
    Problem(Board board, std::vector<Tile> tiles);

    void limit_tile_counts();
    void print() const;

    Board board_;
    std::vector<Tile> tiles_;
};

#endif  // TILER_PROBLEM_PROBLEM_HPP_
