#ifndef TILER_ERRORS_NOT_IMPLEMENTED_ERROR_HPP_
#define TILER_ERRORS_NOT_IMPLEMENTED_ERROR_HPP_

#include <stdexcept>
#include <string>

class NotImplementedError : public std::runtime_error {
public:
    explicit NotImplementedError(const std::string& err) : std::runtime_error(err){};
};

#endif  // TILER_ERRORS_NOT_IMPLEMENTED_ERROR_HPP_
