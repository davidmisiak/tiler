#ifndef TILER_PARSERS_REGION_PARSER_HPP_
#define TILER_PARSERS_REGION_PARSER_HPP_

#include <string>

#include "problem/tile.hpp"

namespace region_parser {

// Parses the shape definition (name, dimensions or map) and returns corresponding Region.
// No extra-whitespace stripping nor continuity/hole checks are done - use this function
// carefully and prefer `Region::parse` if possible.
Region parse_raw(std::string s);

// Parses the shape definition (name, dimensions or map) and returns corresponding Region.
// Extra spaces around map-defined shapes are stripped and a continuity and hole check is
// performed (resulting in an error if failed).
Region parse(std::string s);

}  // namespace region_parser

#endif  // TILER_PARSERS_REGION_PARSER_HPP_
