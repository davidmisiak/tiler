#include "problem/problem.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "parse_error.hpp"
#include "problem/board.hpp"
#include "problem/region.hpp"
#include "problem/tile.hpp"

Problem::Problem(Board board, std::vector<Tile> tiles) : board_(board), tiles_(tiles) {}

Problem Problem::create(std::vector<std::string> definitions, bool reflection) {
    Board board = Board::parse(definitions[0]);
    std::vector<Tile> tiles;
    for (size_t i = 1; i < definitions.size(); i++) {
        tiles.push_back(Tile::parse(definitions[i], reflection));
    }
    return Problem(board, tiles);
}

Problem Problem::create(std::string filepath, bool reflection) {
    std::string s;
    {
        std::ifstream file(filepath);
        std::stringstream ss;
        ss << file.rdbuf();
        s = ss.str();
    }
    std::vector<std::string> definitions;
    const std::regex exp("((.|.\n)+)\n*(\n\n|$)");
    std::smatch matches;
    std::string::const_iterator search_start(s.cbegin());
    while (std::regex_search(search_start, s.cend(), matches, exp)) {
        definitions.push_back(matches[1]);
        search_start = matches.suffix().first;
    }
    if (definitions.size() >= 2) {
        return Problem::create(definitions, reflection);
    }
    throw ParseError("Not enough shapes defined (at least 2 required).");
}

std::ostream &operator<<(std::ostream &os, const Problem &problem) {
    os << problem.board_ << "\n\nTiles:";
    for (Tile tile : problem.tiles_) {
        os << "\n\n" << tile;
    }
    return os;
}
