#ifndef TILER_PRINT_HPP_
#define TILER_PRINT_HPP_

#include <iostream>

#include "rang.hpp"

// Usage example: print::error() << "Error message" << std::endl;
// Proper solution would be to define our own ostream which resets the style after the << operation,
// but as long as we always use one of these functions instead of std::cout and don't forget to call
// print::normal() before exiting the program, this solution works fine as well.
namespace print {

inline std::ostream &normal() { return std::cout << rang::style::reset; }

inline std::ostream &error() { return std::cout << rang::fg::red; }

inline std::ostream &help() { return std::cout << rang::fg::blue; }

inline std::ostream &success() { return std::cout << rang::fg::green; }

inline std::ostream &warning() { return std::cout << rang::fg::yellow; }

}  // namespace print

#endif  // TILER_PRINT_HPP_
