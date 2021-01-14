#ifndef TILER_PROBLEM_PROBLEM_HPP_
#define TILER_PROBLEM_PROBLEM_HPP_

#include <ostream>
#include <string>
#include <vector>

#include "problem/board.hpp"
#include "problem/tile.hpp"

// Represents the problem assignment - board and list of tiles (with their counts).
class Problem {
public:
    Problem() = delete;
    Problem(Board board, std::vector<Tile> tiles);
    static Problem create(std::vector<std::string> shapes, bool reflection);
    static Problem create(std::string filename, bool reflection);
    friend std::ostream &operator<<(std::ostream &os, const Problem &problem);

private:
    Board board_;
    std::vector<Tile> tiles_;
};

#endif  // TILER_PROBLEM_PROBLEM_HPP_
