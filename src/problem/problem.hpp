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

    // Takes list of strings - shape definitions.
    static Problem create(std::vector<std::string> definitions, bool reflection);

    // Takes shape definitions (separated by at least one empty line) in a single string.
    static Problem create(std::string content, bool reflection);

    // Reads defintions from a text file.
    static Problem create_from_file(std::string filepath, bool reflection);

    friend std::ostream &operator<<(std::ostream &os, const Problem &problem);

    Board board_;
    std::vector<Tile> tiles_;
};

#endif  // TILER_PROBLEM_PROBLEM_HPP_
