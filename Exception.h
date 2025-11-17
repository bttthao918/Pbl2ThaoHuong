#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <exception>

class ParkingException : public std::exception {
private:
    std::string message;
public:
    ParkingException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class NotFoundException : public ParkingException {
public:
    NotFoundException(const std::string& msg) : ParkingException(msg) {}
};

class DuplicateException : public ParkingException {
public:
    DuplicateException(const std::string& msg) : ParkingException(msg) {}
};

class FullCapacityException : public ParkingException {
public:
    FullCapacityException(const std::string& msg) : ParkingException(msg) {}
};

class InvalidDataException : public ParkingException {
public:
    InvalidDataException(const std::string& msg) : ParkingException(msg) {}
};

#endif