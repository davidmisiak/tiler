#ifndef TILER_ERRORS_TIME_LIMIT_ERROR_HPP_
#define TILER_ERRORS_TIME_LIMIT_ERROR_HPP_

#include <stdexcept>
#include <string>

class TimeLimitError : public std::runtime_error {
public:
    explicit TimeLimitError() : std::runtime_error("Time limit reached."){};
};

#endif  // TILER_ERRORS_TIME_LIMIT_ERROR_HPP_
