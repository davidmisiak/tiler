#ifndef TILER_PARSERS_TILE_PARSER_HPP_
#define TILER_PARSERS_TILE_PARSER_HPP_

#include <string>

#include "problem/tile.hpp"

namespace tile_parser {

// Parses the tile definitions, consisting of a shape definition optionally prefixed with 'N:'
// where N is the number of available pieces (otherwise set to infinity).
Tile parse(std::string s, bool reflection);

}  // namespace tile_parser

#endif  // TILER_PARSERS_TILE_PARSER_HPP_
