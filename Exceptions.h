#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

using namespace std;

class InvalidInputException : public runtime_error
{
public:
    explicit InvalidInputException(const string &msg)
        : runtime_error(msg) {}
};

class NotFoundException : public runtime_error
{
public:
    explicit NotFoundException(const string &msg)
        : runtime_error(msg) {}
};

class DuplicateException : public runtime_error
{
public:
    explicit DuplicateException(const string &msg)
        : runtime_error(msg) {}
};

#endif
