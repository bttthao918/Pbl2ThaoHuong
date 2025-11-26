#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class InvalidInputException : public std::runtime_error
{
public:
    explicit InvalidInputException(const std::string &msg)
        : std::runtime_error(msg) {}
};

class NotFoundException : public std::runtime_error
{
public:
    explicit NotFoundException(const std::string &msg)
        : std::runtime_error(msg) {}
};

class DuplicateException : public std::runtime_error
{
public:
    explicit DuplicateException(const std::string &msg)
        : std::runtime_error(msg) {}
};

#endif
