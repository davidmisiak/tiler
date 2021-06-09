#ifndef TILER_PRINT_HPP_
#define TILER_PRINT_HPP_

#include <iostream>

#include "rang.hpp"

// Usage example: print::error() << "Error message" << std::endl;
// Proper solution would be to define our own ostream which resets the style after the << operation,
// but as long as we always use one of these functions instead of std::cout and don't forget to call
// print::normal() before exiting the program, this solution works fine as well.
namespace print {

// Normal text.
inline std::ostream &normal() { return std::cout << rang::style::reset; }

// Headings.
inline std::ostream &normal_bold() { return std::cout << rang::style::reset << rang::style::bold; }

// Error messages.
inline std::ostream &error() { return std::cout << rang::style::reset << rang::fg::red; }

// Board/tile shape description.
inline std::ostream &shape() { return std::cout << rang::style::reset << rang::fg::cyan; }

// Solving stats.
inline std::ostream &stats() { return std::cout << rang::style::reset << rang::fg::magenta; }

inline std::ostream &success_bold() { return std::cout << rang::style::bold << rang::fg::green; }

inline std::ostream &warning_bold() { return std::cout << rang::style::bold << rang::fg::yellow; }

}  // namespace print

#endif  // TILER_PRINT_HPP_
