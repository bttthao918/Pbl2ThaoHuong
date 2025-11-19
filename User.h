#ifndef USER_H
#define USER_H
#include "Exceptions.h"
#include <string>
#include <iostream>

using namespace std;  // <<< THÊM VÀO

enum class UserRole
{
    CUSTOMER,
    ADMIN
};

class User
{
protected:
    string userId;
    string username;
    string password;
    string fullName;
    string phoneNumber;
    string email;
    UserRole role;

public:
    User();
    User(const string &id, const string &uname, const string &pwd,
         const string &name, const string &phone, const string &mail, UserRole r);

    virtual ~User() {}

    string getUserId() const { return userId; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getFullName() const { return fullName; }
    string getPhoneNumber() const { return phoneNumber; }
    string getEmail() const { return email; }
    UserRole getRole() const { return role; }

    void setPassword(const string &pwd) { password = pwd; }
    void setFullName(const string &name) { fullName = name; }
    void setPhoneNumber(const string &phone);
    void setEmail(const string &mail);

    virtual void displayInfo() const;
    virtual string toFileString() const;
    virtual void fromFileString(const string &line);

    friend ostream &operator<<(ostream &os, const User &user);
    bool operator==(const User &other) const;
};

#endif
