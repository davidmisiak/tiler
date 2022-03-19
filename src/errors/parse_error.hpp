#ifndef TILER_ERRORS_PARSE_ERROR_HPP_
#define TILER_ERRORS_PARSE_ERROR_HPP_

#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& err) : std::runtime_error(err){};
};

#endif  // TILER_ERRORS_PARSE_ERROR_HPP_
