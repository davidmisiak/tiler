#ifndef TILER_ERRORS_SOLVE_ERROR_HPP_
#define TILER_ERRORS_SOLVE_ERROR_HPP_

#include <stdexcept>
#include <string>

class SolveError : public std::runtime_error {
public:
    explicit SolveError(const std::string& err) : std::runtime_error(err){};
};

#endif  // TILER_ERRORS_SOLVE_ERROR_HPP_
