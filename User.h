#ifndef USER_H
#define USER_H
#include "Exceptions.h"
#include <string>
#include <iostream>

enum class UserRole
{
    CUSTOMER,
    ADMIN
};

class User
{
protected:
    std::string userId;
    std::string username;
    std::string password;
    std::string fullName;
    std::string phoneNumber;
    std::string email;
    UserRole role;

public:
    User();
    User(const std::string &id, const std::string &uname, const std::string &pwd,
         const std::string &name, const std::string &phone, const std::string &mail, UserRole r);

    virtual ~User() {}

    std::string getUserId() const { return userId; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::string getFullName() const { return fullName; }
    std::string getPhoneNumber() const { return phoneNumber; }
    std::string getEmail() const { return email; }
    UserRole getRole() const { return role; }

    void setPassword(const std::string &pwd) { password = pwd; }
    void setFullName(const std::string &name) { fullName = name; }
    void setPhoneNumber(const std::string &phone);
    void setEmail(const std::string &mail);

    virtual void displayInfo() const;
    virtual void displayTableRow() const = 0;
    virtual std::string toFileString() const;
    virtual void fromFileString(const std::string &line);

    friend std::ostream &operator<<(std::ostream &os, const User &user);
    bool operator==(const User &other) const;
};

#endif
