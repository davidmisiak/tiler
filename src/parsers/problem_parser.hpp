#ifndef TILER_PARSERS_PROBLEM_PARSER_HPP_
#define TILER_PARSERS_PROBLEM_PARSER_HPP_

#include <string>
#include <vector>

#include "problem/problem.hpp"

namespace problem_parser {

// Takes list of strings - shape definitions.
Problem parse(std::vector<std::string> definitions, bool reflection);

// Takes shape definitions (separated by at least one empty line) in a single string.
Problem parse(std::string content, bool reflection);

// Reads defintions from a text file.
Problem parse_from_file(std::string filepath, bool reflection);

}  // namespace problem_parser

#endif  // TILER_PARSERS_PROBLEM_PARSER_HPP_
