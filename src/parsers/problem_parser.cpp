#include "parsers/problem_parser.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "parse_error.hpp"
#include "parsers/region_parser.hpp"
#include "parsers/tile_parser.hpp"
#include "problem/board.hpp"
#include "problem/problem.hpp"
#include "problem/tile.hpp"

Problem problem_parser::parse(std::vector<std::string> definitions, bool reflection) {
    Board board(region_parser::parse(definitions[0]));
    std::vector<Tile> tiles;
    for (size_t i = 1; i < definitions.size(); i++) {
        tiles.push_back(tile_parser::parse(definitions[i], reflection));
    }
    return Problem(board, tiles);
}

Problem problem_parser::parse(std::string content, bool reflection) {
    std::vector<std::string> definitions;
    // definitions are separated by at least two '\n' characters (ie. at least one empty line)
    const std::regex exp("((.|.\n)+)\n*(\n\n|$)");
    std::smatch matches;
    std::string::const_iterator search_start(content.cbegin());
    while (std::regex_search(search_start, content.cend(), matches, exp)) {
        definitions.push_back(matches[1]);
        search_start = matches.suffix().first;
    }
    if (definitions.size() >= 2) {
        return parse(definitions, reflection);
    }
    throw ParseError("Not enough shapes defined (at least 2 required).");
}

Problem problem_parser::parse_from_file(std::string filepath, bool reflection) {
    std::string s;
    {
        std::ifstream file(filepath);
        std::stringstream ss;
        ss << file.rdbuf();
        s = ss.str();
    }
    return parse(s, reflection);
}
